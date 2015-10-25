#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

/**
* Check if file exist
*/
int check_file_exist(const char *file){
	 struct stat buffer;
	 int exist = stat(file, &buffer);
	 if(exist == 0)
		  return 1;
	 else
		  return 0;
}

/**
* load sample data to tree
*/
void load_sample_2_tree(BTA *tree, const char *sample_data){
  if(tree == NULL){
    fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
    exit(1);
  }
  FILE *file;
  file = fopen(sample_data, "r");
  if(file == NULL){
    fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
    exit(1);
  }
	 
  int n = 0;
  char word[100], mean[100000];
  char temp[100];

  while(!feof(file)){
      fgets(temp, 100, file);
      if(temp[0] != '\t' && temp[0] != '\n'){ 
          if(n==0){ //read the first word
              n++;
              strcpy(word, temp);
              word[strlen(temp)-1] = '\0';
          }
          else {
              mean[strlen(mean)-1] = '\0';
              btins(tree, word, mean, strlen(mean)+1);
              strcpy(mean, ""); 
              n++;
              strcpy(word, temp);
              word[strlen(temp)-1] = '\0';
              
            }
        } 
      else strcat(mean, temp);

    }
  mean[strlen(mean)-1] = '\0';
  btins(tree, word, mean, strlen(mean)+1); //put in tree
	
  fclose(file);
}

/**
* load data from word tree to soundex tree
*/
void load_soundex_db(BTA *soundex_t, BTA *word_t){
  if(word_t == NULL ||  soundex_t == NULL){
    fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
    exit(1);
  }

  // set tree word position
  btpos(word_t, ZSTART);

  while(1) {
    char word[200];
    char mean[200000];
    int rsize, rsize_soundex;

    if(btseln(word_t, word, mean, sizeof(mean), &rsize) == QNOKEY) 
      break; //break if there is no next key in word_t

    char *soundex_str = soundex(word);
    char series_word[200000];
    if(btsel(soundex_t, soundex_str, series_word, sizeof(series_word), &rsize_soundex) != 0){
      // soundex_str does not exist on soundex_tree 
      btins(soundex_t, soundex_str, word, strlen(word) + 1);
    } else {
      //  soundex_str exists on soundex_tree 
      char separated[100] = ";"; //concat the words that have the same soundex
      strcat(separated, word);
      strcat(series_word, separated);
      btupd(soundex_t, soundex_str, series_word, strlen(series_word) + 1); //update 
    }
    free(soundex_str);
  }
  btpos(word_t, ZSTART);

}

/**
* soundex algorithm
*/
char *soundex(const char *in) {
  int code[] =
    {  0,1,2,3,0,1,2,0,0,2,2,4,5,5,0,1,2,6,2,3,0,1,0,2,0,2 };
  /* a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z */
  char *key = malloc(sizeof(strlen(in)));
  register char ch;
  register int last;
  register int count;

  /* Set up default key, complete with trailing '0's */
  strcpy(key, "Z000");

  /* Advance to the first letter.  If none present, 
     return default key */
  while (*in != '\0'  &&  !isalpha(*in))
    ++in;
  if (*in == '\0')
    return key;

  /* Pull out the first letter, uppercase it, and 
     set up for main loop */
  key[0] = toupper(*in);
  last = code[key[0] - 'A'];
  ++in;

  /* Scan rest of string, stop at end of string or 
     when the key is full */
  for (count = 1;  count < 4  &&  *in != '\0';  ++in) {
    /* If non-alpha, ignore the character altogether */
    if (isalpha(*in)) {
      ch = tolower(*in);
      /* Fold together adjacent letters sharing the same code */
      if (last != code[ch - 'a']) {
        last = code[ch - 'a'];
        /* Ignore code==0 letters except as separators */
        if (last != 0)
          key[count++] = '0' + last;
      }
    }
  }

  return key;
}

/**
* add a word to tree
*/
int add_word_to_dict(ChData *data, char *word, char *mean){
  if (word == NULL || mean == NULL || data == NULL){
    fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
    exit(1);
  }
	
  char mean_rev[2000000];
  int rsize;

  if(btsel(data->tree_word, word, mean_rev, sizeof(mean_rev), &rsize) != 0){
    // word does not exist on tree_word
    if(btins(data->tree_word, word, mean, strlen(mean) + 1) != 0) //error occurred
      return 0;

    char *soundex_str = soundex(word);
    char series_word[200000];
    int rsize_soundex;

    if(btsel(data->tree_soundex, soundex_str, series_word, sizeof(series_word), &rsize_soundex) != 0){
      // soundex_str does not exist on soundex_tree 
      if(btins(data->tree_soundex, soundex_str, word, strlen(word) + 1) != 0) //error occurred
        return 0;
    } else {
      // soundex_str exists on soundex_tree 
      char separated[100] = ";";
      strcat(separated, word);
      strcat(series_word, separated);
      if(btupd(data->tree_soundex, soundex_str, series_word, strlen(series_word) + 1) != 0) //error occurred
        return 0;
    }
    free(soundex_str);
    return 1; //successfully added
  } 

  return 0;
}

/**
* edit a word's meaning
*/
int edit_meaning_from_dict(ChData *data, char *word, char *mean){
	if (word == NULL || mean == NULL || data == NULL){
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}
	
	char mean_rev[2000000];
	int rsize;

	if(btsel(data->tree_word, word, mean_rev, sizeof(mean_rev), &rsize) == 0){
		// word exists on tree_word
		if(btupd(data->tree_word, word, mean, strlen(mean) + 1) != 0)
			return 0;
		return 1; //successfully edited word
	} 
	return 0;
}

/**
* detele a word
*/
int delete_word_from_dict(ChData *data, char *word){
	if (word == NULL || data == NULL){
		fprintf(stderr, "ERROR: NULL value %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}

	char mean_rev[2000000];
	int rsize;

	if(btsel(data->tree_word, word, mean_rev, sizeof(mean_rev), &rsize) == 0){
		// word exists on tree_word
		if(btdel(data->tree_word, word) != 0) //error
			return 0;

		char *soundex_str = soundex(word);
		char series_word[200000];
		int rsize;

		if(btsel(data->tree_soundex, soundex_str, series_word, sizeof(series_word), &rsize) != 0){
			// soundex_str does not exists on soundex_tree 
			fprintf(stderr, "ERROR: Word does not exist in soundex %s:%d\n", __FILE__, __LINE__);
			return TRUE;
		} else {
			// soundex_str exists on soundex_tree 

			char str[1000];
			int count = 0;

			strcpy(str, series_word);
			strcpy(series_word, "");
			char *cut = strtok (str, ";");
			while (cut != NULL)  { 
				if(count == 0)
					if(strcmp(cut, word) != 0)
						strcat(series_word, cut);
				char separated[100] = ";";
				if(strcmp(cut, word) == 0){
					cut = strtok (NULL, ";");
					continue;
				}
				strcat(separated, cut);
				strcat(series_word, separated);
				cut = strtok (NULL, ";");
				count++;
				if (count == 50)
				{
					break;
				}
			}

			if(count == 1)
				if(btdel(data->tree_soundex, soundex_str) != 0){
					return FALSE;
					goto end;
				}

			if(btupd(data->tree_soundex, soundex_str, series_word, strlen(series_word) + 1) != 0)
				return FALSE;
		}
end:
		free(soundex_str);
		return TRUE;
	} 
	return FALSE;
}

// Insert word into history tree view
void insert_2_history(ChData *data){
    gtk_list_store_clear(data->list_store2);
    GtkTreeIter iter;
    node *temp;
    temp = (node*)malloc(sizeof(node));
    temp = data->stack;
    while(temp != NULL){
      //gtk_list_store_append(data->list_store2, &iter);
      //gtk_list_store_set(data->list_store2, &iter, 0, temp->key, -1);
      printf("%s\n", temp->key);
      temp = temp->next;
    }
    printf("=======================\n");
    free(temp);
}
/**
* Clear main window's entry
*/
void reset_entry(GtkWidget *Entry)
{
	GtkEntryBuffer *buffer = gtk_entry_buffer_new (NULL, 0);
	gtk_entry_set_buffer( (GtkEntry*) Entry, buffer);
}

/* Clear the TextView */
void reset_textview(GtkWidget *TextView)
{
	GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
	gtk_text_view_set_buffer( (GtkTextView*) TextView, buffer);
}

void status_dialog(GtkWindow *parent, gchar *message)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(parent,
		GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, message);
	gtk_window_set_title(GTK_WINDOW(dialog), "Status");
	gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
