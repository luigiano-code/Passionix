#include <gtk/gtk.h>
#include <adwaita.h>

#include "comparegtk.h"

GtkWidget *create_compare_page(GCallback login_next, gpointer user_data)
{
    GtkWidget *box;
	GtkWidget *bottom_box_lower;
	GtkWidget *done_button;
	GtkWidget *scroll_box;
	GtkWidget *scroll;

    box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        10
    );
    gtk_widget_set_vexpand(
        box,
        TRUE
    );

    scroll_box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        0
    );
	gtk_widget_set_vexpand(
        scroll_box,
        TRUE
    );
    gtk_widget_set_halign(
        scroll_box,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_valign(
        scroll_box,
        GTK_ALIGN_CENTER
    );

    bottom_box_lower = gtk_box_new(
        GTK_ORIENTATION_HORIZONTAL,
        10
    );
    gtk_widget_set_halign(
        bottom_box_lower,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_valign(
        bottom_box_lower,
        GTK_ALIGN_CENTER
    );

	
	scroll = gtk_scrolled_window_new();	
	gtk_scrolled_window_set_child(
		GTK_SCROLLED_WINDOW(scroll),
		scroll_box
	);	

    done_button = gtk_button_new_with_label("Done");
    gtk_widget_set_halign(
        done_button,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_size_request(
        done_button,
        200,
        60
    );
    gtk_widget_add_css_class(
        done_button,
        "suggested-action"
    );
	g_signal_connect(
		done_button,
		"clicked",
		login_next,
		user_data
	);

    gtk_box_append(
        GTK_BOX(bottom_box_lower),
        done_button
    );

    gtk_box_append(
        GTK_BOX(box),
        scroll
    );

    gtk_box_append(
        GTK_BOX(box),
        bottom_box_lower
    );
    return box;
}
