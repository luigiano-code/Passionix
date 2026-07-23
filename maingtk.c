#include <gtk/gtk.h>
#include <adwaita.h>

#include "maingtk.h"

GtkWidget *create_main_page(GCallback next_callback, gpointer user_data)
{
    GtkWidget *box;
    GtkWidget *bottom_box_upper;
	GtkWidget *bottom_box_lower;
	GtkWidget *add_button;
	GtkWidget *scroll_box;
	GtkWidget *password_button;
    GtkWidget *random_button;
    GtkWidget *compare_button;
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

    bottom_box_upper = gtk_box_new(
        GTK_ORIENTATION_HORIZONTAL,
        10
    );
    gtk_widget_set_halign(
        bottom_box_upper,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_valign(
        bottom_box_upper,
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
		next_callback,
		user_data
	);

    random_button = gtk_button_new_with_label("Generate passwords");
    gtk_widget_set_halign(
        random_button,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_size_request(
        random_button,
        200,
        60
    );
	g_signal_connect(
		random_button,
		"clicked",
		next_callback,
		user_data
	);

    compare_button = gtk_button_new_with_label("Compare saves");
    gtk_widget_set_halign(
        compare_button,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_size_request(
        compare_button,
        200,
        60
    );
	g_signal_connect(
		compare_button,
		"clicked",
		next_callback,
		user_data
	);

    password_button = gtk_button_new_with_label("Change password");
    gtk_widget_set_halign(
        password_button,
        GTK_ALIGN_CENTER
    );
    gtk_widget_set_size_request(
        password_button,
        200,
        60
    );
	g_signal_connect(
		password_button,
		"clicked",
		next_callback,
		user_data
	);

    gtk_box_append(
        GTK_BOX(bottom_box_upper),
        password_button
    );


    gtk_box_append(
        GTK_BOX(bottom_box_upper),
        random_button
    );


    gtk_box_append(
        GTK_BOX(bottom_box_lower),
        compare_button
    );

    gtk_box_append(
        GTK_BOX(bottom_box_lower),
        add_button
    );

    gtk_box_append(
        GTK_BOX(box),
        scroll
    );

    gtk_box_append(
        GTK_BOX(box),
        bottom_box_upper
    );
    gtk_box_append(
        GTK_BOX(box),
        bottom_box_lower
    );
    return box;
}
