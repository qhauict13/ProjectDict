#ifndef __MODEL__
#define __MODEL__
#include <sys/stat.h>
#include <gtk/gtk.h>
#include "support.h"


int check_file_exist(const char *file);


void load_sample_2_tree(BTA *tree, const char *sample_data);


void load_soundex_db(BTA *soundex_t, BTA *word_t);


char *soundex(const char *in);


int add_word_to_dict(ChData *data, char *word, char *mean);


int edit_meaning_from_dict(ChData *data, char *word, char *mean);


int delete_word_from_dict(ChData *data, char *word);


void reset_entry(GtkWidget *Entry);

void reset_textview(GtkWidget *TextView);

void status_dialog(GtkWindow *parent, gchar *message);

#endif
