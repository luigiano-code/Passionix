#include <stdio.h>
#include <gtk/gtk.h>
#include <adwaita.h>

#include "data.h"
#include "user.h"

int data_index = 0;
Dictionary data[128];
char* main_password;

void list_data(GtkWidget *scroll_box, int readonly_mode)
{
    for (int i = 0; i < data_index; i++)
    {
        GtkWidget *box = gtk_box_new(
            GTK_ORIENTATION_HORIZONTAL,
            10
        );

        gtk_widget_set_valign(
            box,
            GTK_ALIGN_CENTER
        );

        if (readonly_mode == 0)
        {
            GtkWidget *username_label = gtk_label_new(data[i].key);

            gtk_widget_set_halign(
                username_label,
                GTK_ALIGN_CENTER
            );
            gtk_widget_add_css_class(
                username_label,
                "title-2"
            );
				
			GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
			gtk_widget_set_hexpand(spacer, TRUE);

            GtkWidget *edit_button = gtk_button_new_with_label("Edit");
            gtk_widget_set_halign(
                edit_button,
                GTK_ALIGN_CENTER
            );
			g_signal_connect(
				edit_button,
				"clicked",
				G_CALLBACK(edit_clicked),
				GINT_TO_POINTER(i)
			);

            GtkWidget *copy_button = gtk_button_new_with_label("Copy");
            gtk_widget_set_halign(
                copy_button,
                GTK_ALIGN_CENTER
            );
			g_signal_connect(
				copy_button,
				"clicked",
				G_CALLBACK(copy_clicked),
				GINT_TO_POINTER(i)
			);

            GtkWidget *delete_button = gtk_button_new_with_label("Delete");
            gtk_widget_set_halign(
                delete_button,
                GTK_ALIGN_CENTER
            );
			g_signal_connect(
				delete_button,
				"clicked",
				G_CALLBACK(delete_clicked),
				GINT_TO_POINTER(i)
			);

			gtk_widget_set_margin_start(box, 20);
			gtk_widget_set_margin_end(box, 20);

            gtk_box_append(
                GTK_BOX(box),
                username_label
            );

            gtk_box_append(
                GTK_BOX(box),
                spacer
            );

            gtk_box_append(
                GTK_BOX(box),
                edit_button
            );

            gtk_box_append(
                GTK_BOX(box),
                copy_button
            );
            gtk_box_append(
                GTK_BOX(box),
                delete_button
            );
        }

        gtk_box_append(
            GTK_BOX(scroll_box),
            box
        );
    }
}
