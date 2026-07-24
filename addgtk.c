#include <gtk/gtk.h>
#include <adwaita.h>

#include "addgtk.h"

GtkWidget *create_add_page(GCallback add_callback, gpointer user_data)
{
    GtkWidget *box;
    GtkWidget *center_box;
	GtkWidget *username_entry;
    GtkWidget *password_entry;
    GtkWidget *add_button;
    GtkWidget *greeter;

    box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        10
    );
    gtk_widget_set_vexpand(
        box,
        TRUE
    );

    center_box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        0
    );
    gtk_widget_set_vexpand(
        center_box,
        TRUE
    );
    gtk_widget_set_halign(
        center_box,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_valign(
        center_box,
        GTK_ALIGN_CENTER
    );

    greeter = gtk_label_new("Register new account");
    gtk_widget_set_halign(
        greeter,
        GTK_ALIGN_CENTER
    );
    gtk_widget_add_css_class(
        greeter,
        "title-1"
    );


    username_entry = gtk_entry_new();
    gtk_widget_set_halign(
        username_entry,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_size_request(
        username_entry,
        200,
        30
    );
	gtk_widget_set_margin_top(username_entry, 50);
	gtk_widget_set_margin_bottom(username_entry, 10);

    password_entry = gtk_password_entry_new();
    gtk_widget_set_halign(
        password_entry,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_size_request(
        password_entry,
        200,
        30
    );
	gtk_widget_set_margin_bottom(password_entry, 50);
	gtk_widget_set_margin_top(username_entry, 10);

	AddContext *ctx = malloc(sizeof(AddContext));

	ctx->entry = username_entry;
	ctx->password_entry = password_entry;

    add_button = gtk_button_new_with_label("Add");
    gtk_widget_set_halign(
        add_button,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_size_request(
        add_button,
        200,
        60
    );
    gtk_widget_add_css_class(
        add_button,
        "suggested-action"
    );
	g_signal_connect(
		add_button,
		"clicked",
		add_callback,
		ctx
	);

    gtk_box_append(
        GTK_BOX(center_box),
        greeter
    );

    gtk_box_append(
        GTK_BOX(center_box),
		username_entry
    );

    gtk_box_append(
        GTK_BOX(center_box),
        password_entry
    );

    gtk_box_append(
        GTK_BOX(center_box),
        add_button
    );

    gtk_box_append(
        GTK_BOX(box),
        center_box
    );

    return box;
}
