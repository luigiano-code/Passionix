#include <gtk/gtk.h>
#include <adwaita.h>

#include "logingtk.h"

void create_login_window(GtkApplication *app, gpointer user_data)
{
    AdwApplicationWindow *window;
    AdwHeaderBar *header;

    GtkWidget *toolbar_view;
    GtkWidget *box;
    GtkWidget *center_box;
    GtkWidget *password_entry;
    GtkWidget *next_button;
    GtkWidget *greeter;


    window = ADW_APPLICATION_WINDOW(
        adw_application_window_new(app)
    );


    gtk_window_set_title(
        GTK_WINDOW(window),
        "Passionix"
    );
    gtk_window_set_default_size(
        GTK_WINDOW(window),
        600,
        400
    );


    header = ADW_HEADER_BAR(
        adw_header_bar_new()
    );
    adw_header_bar_set_title_widget(
        header,
        gtk_label_new("Passionix")
    );


    toolbar_view = adw_toolbar_view_new();
    adw_toolbar_view_add_top_bar(
        ADW_TOOLBAR_VIEW(toolbar_view),
        GTK_WIDGET(header)
    );


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
	gtk_widget_set_margin_top(greeter, 50);
	gtk_widget_set_margin_bottom(greeter, 10);


    password_entry = gtk_password_entry_new();
    gtk_widget_set_halign(
        password_entry,
        GTK_ALIGN_CENTER
    );
	gtk_widget_set_valign(
        password_entry,
        GTK_ALIGN_CENTER
    );
	gtk_widget_set_size_request(
        password_entry,
        200,
        30
    );
	gtk_widget_set_margin_top(password_entry, 10);
	gtk_widget_set_margin_bottom(password_entry, 10);


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
    );
    gtk_widget_set_margin_bottom(
        next_button,
        30
    );


    gtk_box_append(
        GTK_BOX(box),
        greeter
    );

    gtk_box_append(
        GTK_BOX(center_box),
        password_entry
    );

    gtk_box_append(
        GTK_BOX(box),
        center_box
    );

    gtk_box_append(
        GTK_BOX(box),
        next_button
    );

    adw_toolbar_view_set_content(
        ADW_TOOLBAR_VIEW(toolbar_view),
        box
    );


    adw_application_window_set_content(
        window,
        toolbar_view
    );


    gtk_window_present(
        GTK_WINDOW(window)
    );
}
