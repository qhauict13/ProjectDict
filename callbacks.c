#include "support.h"
#include "model.h"
#include <stdlib.h>
#include <string.h>
#define MAX_SUGGEST 30

/* Stop application when closed */
G_MODULE_EXPORT void on_main_window_destroy(GtkWidget *main_window)
{
	gtk_main_quit();
}

//************************
//     Main Dialog        
//************************

 // display meaning when a word is selected from tree selection
G_MODULE_EXPORT void on_changed_main(GtkTreeSelection *treeselection, ChData *data) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *word;
  int i;
  if (gtk_tree_selection_get_selected(treeselection, &model, &iter)) {
	gtk_tree_model_get(model, &iter, 0, &word,  -1); //get word from selected node
	int rsize;
	char meaning[100000];
	gchar* check;
	GtkTextBuffer *Buffer;
	Buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->txt_meaning_main));
	gtk_text_buffer_set_text(Buffer, "", -1); //set meaning text view to buffer
	if (btsel(data->tree_word, word, meaning, sizeof(meaning), &rsize) == 0){
		gtk_text_buffer_set_text(Buffer, meaning, -1);
		
	}
	g_free(word);
  }
}

G_MODULE_EXPORT void on_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, ChData *data){
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *word;
	printf("called!!!!\n");
	model = gtk_tree_view_get_model(tree_view);
	if(gtk_tree_model_get_iter(model, &iter, path)){
		gtk_tree_model_get(model, &iter, 0, &word, -1);
		push((&data->stack), word);
		insert_2_history(data);
	}
	g_free(word);
}

// triggered when search entry is changed
G_MODULE_EXPORT void on_search_changed_entry_main (GtkSearchEntry *entry, ChData *data){
	gchar *search_word = gtk_entry_get_text(GTK_ENTRY(entry));
	
	GtkTextBuffer *Buffer;
	Buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->txt_meaning_main));
	gtk_text_buffer_set_text(Buffer, "", -1); //get the meaning textview buffer to 'Buffer'
	gtk_list_store_clear(data->list_store1);
	gtk_tree_selection_unselect_all (data->treeview_selection1);

	int rsize;
	char meaning[100000];
	if (btsel(data->tree_word, search_word, meaning, sizeof(meaning), &rsize) == 0){ //search_word exist in tree_word
		
		GtkTreeIter iter;
		gtk_list_store_append(data->list_store1, &iter);
		gtk_list_store_set(data->list_store1, &iter, 0, search_word, -1);
		gtk_text_buffer_set_text(Buffer, meaning, -1); //display meaning right away
		gtk_tree_selection_unselect_iter (data->treeview_selection1, &iter);
	} else {
		char *soundex_str = soundex(search_word);
		char series_word[10000];
		int rsize;
		if (btsel(data->tree_soundex, soundex_str, series_word, sizeof(series_word), &rsize) != 0) //soundex_str does not exist
				return;
		//printf("%s\n", series_word);
		GtkTreeIter iter;
		char str[1000];
		int count = 0;
		strcpy(str, series_word);
		char *cut = strtok (str, ";");
		while (cut != NULL){
			gtk_list_store_append(data->list_store1, &iter);
			gtk_list_store_set (data->list_store1, &iter, 0, cut, -1);
			cut = strtok (NULL, ";");
			count++;
			if(count > (MAX_SUGGEST))
				break;
			
		}
		gtk_tree_selection_unselect_iter (data->treeview_selection1, &iter);
		free(soundex_str);
	}
}



/* HELP button handler */
G_MODULE_EXPORT void on_btn_help_clicked_main(GtkButton *button, ChData *data)
{
		gtk_dialog_run(GTK_DIALOG(data->dlg_help));
		gtk_widget_hide(data->dlg_help);
}

/* ADD button (on main window) handler */
G_MODULE_EXPORT void on_btn_add_clicked_main(GtkButton *button, ChData *data)
{

		gtk_dialog_run(GTK_DIALOG(data->dlg_add));
		gtk_widget_hide(data->dlg_add);
}

/* Edit button (on main window) handler */
G_MODULE_EXPORT void on_btn_edit_clicked_main(GtkButton *button, ChData *data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *word;

	if (gtk_tree_selection_get_selected(data->treeview_selection1, &model, &iter)) {
		gtk_tree_model_get(model, &iter, 0, &word,  -1);
		gtk_label_set_text(GTK_LABEL(data->word_label_edit), word); //set label of edit dialog
		gtk_dialog_run(GTK_DIALOG(data->dlg_edit));
		gtk_widget_hide(data->dlg_edit);
		g_free(word);
	} else {
		gchar *search_word = gtk_entry_get_text(GTK_ENTRY(data->entry_search_main));
		if(strcmp(search_word, "") == 0)
			status_dialog( (GtkWindow*) data->main_window, "Please select a word!");
		else{
			gtk_label_set_text(GTK_LABEL(data->word_label_edit), search_word);
			gtk_dialog_run(GTK_DIALOG(data->dlg_edit));
			gtk_widget_hide(data->dlg_edit);
		}
	}
}

/* DELETE button handler */
G_MODULE_EXPORT void on_btn_delete_clicked_main(GtkButton *button, ChData *data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *word;

	if (gtk_tree_selection_get_selected(data->treeview_selection1, &model, &iter)) {

		gtk_tree_model_get(model, &iter, 0, &word,  -1);
		gtk_dialog_run(GTK_DIALOG(data->dlg_delete));
		gtk_widget_hide(data->dlg_delete);
		g_free(word);
	} else {
		gchar *search_word = gtk_entry_get_text(GTK_ENTRY(data->entry_search_main));
		if(strcmp(search_word, "") == 0)
			status_dialog( (GtkWindow*) data->main_window, "Please select word on list !");
		else{
			gtk_label_set_text(GTK_LABEL(data->word_label_edit), search_word);
			gtk_dialog_run(GTK_DIALOG(data->dlg_delete));
			gtk_widget_hide(data->dlg_delete);
		}
	}

}


//************************
//     Add dialog        
//************************

/* CANCEL button handler */
 G_MODULE_EXPORT void on_btn_cancel_clicked_add(GtkButton *button, ChData *data)
 {
	gtk_widget_hide(data->dlg_add);     
	reset_entry(data->entry_add);
	reset_textview(data->txt_meaning_add);
 }


/* ADD button handler */
G_MODULE_EXPORT void on_btn_add_clicked_add(GtkButton *button, ChData *data)
{
	const gchar *word = gtk_entry_get_text(GTK_ENTRY(data->entry_add));
	if (!strcmp(word, "")){
		status_dialog( (GtkWindow*) data->main_window, "Please enter a word!");
	} else {
		/* Get text from GtkTextView */
		GtkTextIter start, end;
		GtkTextBuffer *buffer = gtk_text_view_get_buffer ( (GtkTextView*) (data->txt_meaning_add) );
		if (!strcmp(buffer, "")){
			status_dialog( (GtkWindow*) data->main_window, "Please enter meaning!");
		} else {
			gchar *meaning;
			gtk_text_buffer_get_bounds (buffer, &start, &end);
			meaning = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

			/* Insert the word into the tree */
			if(add_word_to_dict(data, word, meaning)){
				status_dialog( (GtkWindow*) data->dlg_add, "Successfully added!");
			}else{
				status_dialog( (GtkWindow*) data->dlg_add, "An error has occured!\nPlease try again");
			}
			/* reset text field */
			reset_entry(data->entry_add);
			reset_textview(data->txt_meaning_add);
			gtk_widget_hide(data->dlg_add); 
			/* free memory */
			g_free(meaning);
		}
	}
	//g_free(word);
}
//************************
//    	Edit dialog       
//************************

/* Edit button handler */
G_MODULE_EXPORT void on_btn_edit_clicked_edit(GtkButton *button, ChData *data)
{
	gchar *word = gtk_label_get_text(GTK_LABEL(data->word_label_edit));

	/* Get text from GtkTextView */
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer ( (GtkTextView*) (data->txt_meaning_edit) );
	gchar *meaning;
	gtk_text_buffer_get_bounds (buffer, &start, &end);
	meaning = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

	/* Edit the word into the tree */
	if(edit_meaning_from_dict(data, word, meaning)){
		status_dialog( (GtkWindow*) data->dlg_edit, "Successfully edited!");
	}else{
		status_dialog( (GtkWindow*) data->dlg_edit, "An error has occured!\nPlease try again");
	}

	reset_textview(data->txt_meaning_edit);
	gtk_widget_hide(data->dlg_edit);

	reset_entry(data->entry_search_main);
	gtk_list_store_clear(data->list_store1);
	reset_textview(data->txt_meaning_main);
	
	GtkTextBuffer *Buffer;
	Buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->txt_meaning_main));

	GtkTreeIter iter;
	gtk_list_store_append(data->list_store1, &iter);
	gtk_list_store_set(data->list_store1, &iter, 0, word, -1);
	gtk_text_buffer_set_text(Buffer, meaning, -1); //display meaning right away

	g_free(meaning);
}

/* CANCEL button handler */
 G_MODULE_EXPORT void on_btn_cancel_clicked_edit(GtkButton *button, ChData *data)
 {
	gtk_widget_hide(data->dlg_edit);     
	reset_textview(data->txt_meaning_edit);
 }

//************************
//    	Delete Dialog        
//************************

/* Delete button handler */
G_MODULE_EXPORT void on_btn_delete_clicked_delete(GtkButton *button, ChData *data)
{
	
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *word;
	gint flag = 0;
	if (gtk_tree_selection_get_selected(data->treeview_selection1, &model, &iter)) {
		gtk_tree_model_get(model, &iter, 0, &word,  -1);
		flag = 1;
	} else {
		word = gtk_entry_get_text(GTK_ENTRY(data->entry_search_main));
		if(strcmp(word, "") == 0)
			status_dialog( (GtkWindow*) data->main_window, "Please select word on list !");
	}
	
	/* Edit the word into the tree */
	if(delete_word_from_dict(data, word)){
		status_dialog( (GtkWindow*) data->dlg_delete, "Successfully deleted!");
	}else{
		status_dialog( (GtkWindow*) data->dlg_delete, "An error has occured!\nPlease try again");
	}
	/* reset text field */

	gtk_widget_hide(data->dlg_delete);
	reset_entry(data->entry_search_main);
	gtk_list_store_clear(data->list_store1);

	/* free memory */
	if(flag == 1)
		g_free(word);
	reset_textview(data->txt_meaning_main);
}

/* CANCEL button handler */
 G_MODULE_EXPORT void on_btn_cancel_clicked_delete(GtkButton *button, ChData *data)
 {
	gtk_widget_hide(data->dlg_delete);
 }

//************************
//     Help dialog        
//************************
 G_MODULE_EXPORT void on_btn_close_clicked_help(GtkButton *button, ChData *data)
 {
	gtk_widget_hide(data->dlg_help);     
 }