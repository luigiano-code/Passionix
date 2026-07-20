#include "app.h"
#include <json-glib/json-glib.h>
#include <stdlib.h>
#include <time.h>
#include "crypto.h"
#include "password-operations.h"
#include "data-operations.h"
#include "clipboard-operations.h"
#include "user-operations.h"

#define DATA_FILE "data.json"
#define BACKUP_FILE "old.json"
#define HASH_LOG_FILE "hashes.txt"

void show_main_view(AdwApplication *app, const char *master_password)
{
    srand((unsigned int)time(NULL));

    GHashTable *dict = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    GtkWidget *window = adw_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Main View");
    gtk_window_set_default_size(GTK_WINDOW(window), 420, 420);

    GtkWidget *toolbar = adw_toolbar_view_new();
    GtkWidget *header = adw_header_bar_new();
    adw_toolbar_view_add_top_bar(ADW_TOOLBAR_VIEW(toolbar), header);

    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(content, 20);
    gtk_widget_set_margin_bottom(content, 20);
    gtk_widget_set_margin_start(content, 20);
    gtk_widget_set_margin_end(content, 20);

    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER,
                                   GTK_POLICY_AUTOMATIC);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), content);

    GtkWidget *extra_buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_margin_top(extra_buttons_box, 8);
    gtk_widget_set_margin_bottom(extra_buttons_box, 8);
    gtk_widget_set_margin_start(extra_buttons_box, 8);
    gtk_widget_set_margin_end(extra_buttons_box, 8);
    gtk_widget_set_halign(extra_buttons_box, GTK_ALIGN_CENTER);

    AddData *context = g_new0(AddData, 1);
    context->dict = dict;
    context->content_box = content;
    context->master_password = g_strdup(master_password);

    const char *labels[5] = {"S", "C", "P", "R", "+"};
    for (int i = 0; i < 5; i++) {
        GtkWidget *btn = gtk_button_new_with_label(labels[i]);

        if (g_strcmp0(labels[i], "+") == 0) {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_add_clicked), context);
        } else if (g_strcmp0(labels[i], "S") == 0) {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_save_clicked), context);
        } else if (g_strcmp0(labels[i], "R") == 0) {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_random_clicked), context);
        } else if (g_strcmp0(labels[i], "P") == 0) {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_password_window_clicked), context);
        }

        gtk_box_append(GTK_BOX(extra_buttons_box), btn);
    }

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_box_append(GTK_BOX(main_box), scrolled_window);
    gtk_box_append(GTK_BOX(main_box), extra_buttons_box);

    adw_toolbar_view_set_content(ADW_TOOLBAR_VIEW(toolbar), main_box);
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), toolbar);

    g_object_set_data_full(G_OBJECT(window), "my_dict", dict, (GDestroyNotify)g_hash_table_destroy);
    g_object_set_data_full(G_OBJECT(window), "my_context", context, (GDestroyNotify)g_free);

    load_from_json(content, dict, window, master_password);

    gtk_window_present(GTK_WINDOW(window));
}
