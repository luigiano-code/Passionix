#ifndef MAINGTK
#define MAINGTK

GtkWidget *create_main_page(
    GCallback main_random,
    GCallback main_add,
    GCallback main_password,
	GCallback main_compare,
    gpointer user_data
);

extern GtkWidget *main_scroll_box;

#endif
