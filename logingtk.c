#include <gtk/gtk.h>
#include <adwaita.h>

#include "logingtk.h"

GtkWidget *create_login_page(GtkStack *stack)
{
    GtkWidget *box;
    GtkWidget *center_box;
    GtkWidget *password_entry;
    GtkWidget *next_button;
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

    greeter = gtk_label_new("Type your password");
    gtk_widget_set_halign(
        greeter,
        GTK_ALIGN_CENTER
    );
    gtk_widget_add_css_class(
        greeter,
        "title-1"
    );


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
	gtk_widget_set_margin_top(password_entry, 50);


    next_button = gtk_button_new_with_label("Next");
    gtk_widget_set_halign(
        next_button,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_size_request(
        next_button,
        200,
        60
    );
    gtk_widget_add_css_class(
        next_button,
        "suggested-action"
    );/*
    g_signal_connect(
        next_button,
        "clicked",
        G_CALLBACK(next_clicked),
        stack
    );*/

    gtk_box_append(
        GTK_BOX(center_box),
        greeter
    );

    gtk_box_append(
        GTK_BOX(center_box),
        password_entry
    );

    gtk_box_append(
        GTK_BOX(center_box),
        next_button
    );

    gtk_box_append(
        GTK_BOX(box),
        center_box
    );

    return box;
}
