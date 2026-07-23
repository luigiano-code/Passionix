#include <stdio.h>

#include "user.h"
#include "save.h"
#include "data.h"
#include "maingtk.h"

typedef struct
{
    GtkWidget *entry;
    int index;
} EditContext;

void delete_clicked(GtkButton *button, gpointer user_data)
{
    int index = GPOINTER_TO_INT(user_data);

    for (int i = index; i < data_index - 1; i++)
    {
        data[i] = data[i + 1];
    }

    data_index--;

    save_data("data.json");
	refresh_list();
}

void copy_clicked(GtkButton *button, gpointer user_data)
{
    int index = GPOINTER_TO_INT(user_data);

    GdkClipboard *clipboard = gdk_display_get_clipboard(
        gdk_display_get_default()
    );

    gdk_clipboard_set_text(
        clipboard,
        data[index].value
    );
}

void edit_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *dialog = gtk_window_new();
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    int index = GPOINTER_TO_INT(user_data);

	gtk_window_set_title(GTK_WINDOW(dialog), "Edit");
	gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
	gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

	gtk_window_present(GTK_WINDOW(dialog));

	gtk_window_set_child(GTK_WINDOW(dialog), box);

    GtkWidget *entry = gtk_entry_new();

    gtk_editable_set_text(
        GTK_EDITABLE(entry),
        data[index].value
    );

    GtkWidget *save = gtk_button_new_with_label("Save");


    gtk_box_append(
        GTK_BOX(box),
        entry
    );

    gtk_box_append(
        GTK_BOX(box),
        save
    );


	EditContext *ctx = malloc(sizeof(EditContext));

	ctx->entry = entry;
	ctx->index = index;

	g_signal_connect(
		save,
		"clicked",
		G_CALLBACK(save_clicked),
		ctx
	);
}

void save_clicked(GtkButton *button, gpointer user_data)
{
	EditContext *ctx = user_data;    

	GtkWidget *entry = user_data;

    const char *text = gtk_editable_get_text(GTK_EDITABLE(ctx->entry));

    strcpy(data[ctx->index].value, text);

    free(ctx);
	save_data("data.json");
	refresh_list();
}

void refresh_list(void)
{
    GtkWidget *child;

    while ((child = gtk_widget_get_first_child(main_scroll_box)) != NULL)
    {
        gtk_box_remove(GTK_BOX(main_scroll_box), child);
    }

    list_data(main_scroll_box, 0);
}

int get_choice()
{
	int choice;

	scanf("%d", &choice);

	return choice;
}
