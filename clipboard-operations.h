#ifndef CLIPBOARD_OPERATIONS_H
#define CLIPBOARD_OPERATIONS_H

#include <adwaita.h>
#include <glib.h>

void copy_to_clipboard(GtkWidget *widget, const char *text);
void on_copy_clicked(GtkButton *button, gpointer user_data);

#endif
