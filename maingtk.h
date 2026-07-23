#ifndef MAINGTK
#define MAINGTK

GtkWidget *create_main_page(
    GCallback main_random,
    GCallback main_add,
    GCallback main_password,
    gpointer user_data
);

#endif
