#include <gtk/gtk.h>
#include <adwaita.h>

#include "randomgtk.h"

GtkWidget *create_random_page(GCallback next_callback, gpointer user_data)
{
    GtkWidget *box;
    GtkWidget *center_box;
	GtkWidget *password_length;
	GtkWidget *generate_button;

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

	password_length = gtk_scale_new_with_range(
		GTK_ORIENTATION_HORIZONTAL,
		1,
		25,
		1
	);
	gtk_scale_set_draw_value(
		GTK_SCALE(password_length),
		TRUE
	);

	generate_button = gtk_button_new_with_label("Generate");
	gtk_widget_set_halign(
        generate_button,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_size_request(
        generate_button,
        200,
        60
    );
    gtk_widget_add_css_class(
        generate_button,
        "suggested-action"
    );
	gtk_widget_set_margin_top(generate_button, 50);

    gtk_box_append(
        GTK_BOX(box),
        password_length
    );

    gtk_box_append(
        GTK_BOX(box),
		generate_button
    );

    gtk_box_append(
        GTK_BOX(box),
        center_box
    );

    return box;
}
