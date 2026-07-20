#ifndef USER_OPERATIONS_H
#define USER_OPERATIONS_H

#include <glib.h>
#include <adwaita.h>

void on_add_clicked(GtkButton *button, gpointer user_data);
void on_dialog_destroy(GtkWidget *widget, gpointer user_data);
void on_password_window_clicked(GtkButton *button, gpointer user_data);
void on_set_password_confirm(GtkButton *button, gpointer user_data);
void on_random_clicked(GtkButton *button, gpointer user_data);
void on_random_confirm_clicked(GtkButton *button, gpointer user_data);
void on_save_clicked(GtkButton *button, gpointer user_data);
void on_add_confirm_clicked(GtkButton *button, gpointer user_data);
void add_item_to_list(GtkWidget *content_box, GHashTable *dict, const char *username, GtkWidget *parent_window);
void on_delete_clicked(GtkButton *button, gpointer user_data);
void on_change_pass_confirm(GtkButton *button, gpointer user_data);
void on_change_clicked(GtkButton *button, gpointer user_data);

#endif
