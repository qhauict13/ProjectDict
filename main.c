#include "support.h"
#include "model.h"



     if(check_file_exist("dict_db"))
          tree_word = btopn("dict_db", 0, FALSE);
     else{

     data->tree_word = tree_word;
     data->tree_soundex = tree_soundex;
     data->stack = stack;
     /* Get more objects to use */
#undef GW

     CH_GET_OBJECT(builder, list_store1, GTK_LIST_STORE, data);
     CH_GET_OBJECT(builder, list_store2, GTK_LIST_STORE, data);

     CH_GET_OBJECT(builder, treeview_selection1, GTK_TREE_SELECTION, data);
     CH_GET_OBJECT(builder, treeview_selection2, GTK_TREE_SELECTION, data);
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

