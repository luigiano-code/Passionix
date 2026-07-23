#ifndef USER_H
#define USER_H

#include <gtk/gtk.h>
#include <adwaita.h>

int get_choice();
void delete_clicked(GtkButton *button, gpointer user_data);
void copy_clicked(GtkButton *button, gpointer user_data);
void edit_clicked(GtkButton *button, gpointer user_data);
void save_clicked(GtkButton *button, gpointer user_data);
void refresh_list(void);

#endif
