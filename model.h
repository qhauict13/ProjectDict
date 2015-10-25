#ifndef __MODEL__
#define __MODEL__
#include <sys/stat.h>
#include <gtk/gtk.h>
#include "support.h"

// check exist file
int check_file_exist(const char *file);

// create db file from foldoc data file
void load_sample_2_tree(BTA *tree, const char *sample_data);

// Create soundex db file from word db file
void load_soundex_db(BTA *soundex_t, BTA *word_t);

// soundex find similar string
char *soundex(const char *in);

// Support for Add function in add dialog
int add_word_to_dict(ChData *data, char *word, char *mean);

// Support for edit function in edit dialog
int edit_meaning_from_dict(ChData *data, char *word, char *mean);

// Support for delete function in edit dialog
int delete_word_from_dict(ChData *data, char *word);

/* Clear the Entry */
void reset_entry(GtkWidget *Entry);

/* Clear the TextView */
void reset_textview(GtkWidget *TextView);

void status_dialog(GtkWindow *parent, gchar *message);

#endif
