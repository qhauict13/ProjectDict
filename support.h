#ifndef __SUPPORT__
#define __SUPPORT__

#include <gtk/gtk.h>
#ifdef TRUE //undefine TRUE and FALSE of gtk lib, avoid conflict with btree
#undef TRUE
#undef FALSE
#endif
#include <btree.h>
#include "stack.h"

/* Macros for obtaining objects from UI file */
#define CH_GET_OBJECT( builder, name, type, data ) \
     data->name = type ( gtk_builder_get_object(builder, #name ))
#define CH_GET_WIDGET( builder, name, data ) \
     CH_GET_OBJECT( builder, name, GTK_WIDGET, data )

/* Main data structure definition */
typedef struct _ChData ChData;
struct _ChData
{
     /* Widget */
  GtkWidget *main_window; /* Main application window */
  GtkWidget *dlg_add;     /* Add dialog */
  GtkWidget *dlg_help;   /* About dialog */
  GtkWidget *dlg_delete;  /* Delete dialog */
  GtkWidget *dlg_edit; /* Edit dialog */

  GtkWidget *entry_add; /* Word entry, Add dialog */
  GtkWidget *txt_meaning_add; /* Meaning entry, Add dialog */

  GtkWidget *treeview_result_main; /* Tree view, main window*/
  GtkWidget *entry_search_main; /* Word search, main window */
  GtkWidget *txt_meaning_main; /* Word meaning, main window */

  GtkWidget *word_label_edit; /* Word label, edit dialog */ 
  GtkWidget *txt_meaning_edit; /* Meaning entry, edit dialog */
     
  GtkListStore *list_store1; //datatype for treeview
  GtkListStore *list_store2;
  BTA *tree_word; //  key word, value mean
  BTA *tree_soundex; // key string of soundex, string contain list of word has same string of soundex separated by ";"

  node *stack;

  GtkTreeSelection *treeview_selection1;
  GtkTreeSelection *treeview_selection2;
};

#endif


