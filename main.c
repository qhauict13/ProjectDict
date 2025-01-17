#include "support.h"
#include "model.h"



     if(check_file_exist("dict_db"))
          tree_word = btopn("dict_db", 0, FALSE);
     else{
          tree_word = btcrt("dict_db", 0, FALSE);
          load_sample_2_tree(tree_word, "FOLDOC");
     }

     if(check_file_exist("soundex_db"))
          tree_soundex = btopn("soundex_db", 0, FALSE);
     else{
          tree_soundex = btcrt("soundex_db", 0, FALSE);
          load_soundex_db(tree_soundex, tree_word);
     }
     
     /* Init GTK+ */
     gtk_init(&argc, &argv);

     /* Create new GtkBuilder object */
     builder = gtk_builder_new();
     if (!gtk_builder_add_from_file(builder, UI_FILE, &error))
     {
          g_warning("%s", error->message);
          g_free(error);
          return 1;
     }

     /* Allocate data structure */
     data = g_slice_new(ChData);

     /* Get objects from UI */
#define GW(name) CH_GET_WIDGET(builder, name, data)
     GW(main_window);
     GW(dlg_add);
     GW(dlg_help);
     GW(dlg_delete);
     GW(dlg_edit);


     GW(entry_add);
     GW(txt_meaning_add);
     GW(treeview_result_main);
     GW(entry_search_main);
     GW(txt_meaning_main);
     GW(word_label_edit);
     GW(txt_meaning_edit);
     GW(list_store1);
     GW(list_store2);
     data->tree_word = tree_word;
     data->tree_soundex = tree_soundex;
     data->stack = stack;
     /* Get more objects to use */
#undef GW

     CH_GET_OBJECT(builder, list_store1, GTK_LIST_STORE, data);
     CH_GET_OBJECT(builder, list_store2, GTK_LIST_STORE, data);

     /* Connect signals */
     gtk_builder_connect_signals(builder, data);

     /* Destroy builder, since we don't need it anymore */
     g_object_unref(G_OBJECT(builder));

     /* Show window */
     gtk_widget_show(data -> main_window);

     /* Start main loop */
     gtk_main();

     /* Free any allocated data */
     g_slice_free(ChData, data);
     btcls(tree_soundex);
     btcls(tree_word);
     freeStack(&stack);

     return(0);
}

