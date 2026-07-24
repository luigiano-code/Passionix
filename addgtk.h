#ifndef ADDGTK
#define ADDGTK

GtkWidget *create_add_page(
    GCallback next_callback,
    gpointer user_data
);

typedef struct
{
    GtkWidget *entry;
    GtkWidget *password_entry;
	char *username;
	char *password;
} AddContext;

#endif
