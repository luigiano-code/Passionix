#include "clipboard-operations.h"
#include "app.h"

void copy_to_clipboard(GtkWidget *widget, const char *text)
{
    GdkDisplay *display = gtk_widget_get_display(widget);
    GdkClipboard *clipboard = gdk_display_get_clipboard(display);
    gdk_clipboard_set_text(clipboard, text);
}

void on_copy_clicked(GtkButton *button, gpointer user_data)
{
    RowData *data = (RowData *)user_data;
    const char *password = g_hash_table_lookup(data->dict, data->username);

    if (password) {
        copy_to_clipboard(GTK_WIDGET(button), password);
    }
}
