#include "clipboard-operations.h"
#include "app.h"
#include "data-operations.h"
#include "user-operations.h"
#include "password-operations.h"

void on_add_clicked(GtkButton *button, gpointer user_data)
{
    AddData *context = (AddData *)user_data;
    GtkWidget *parent_window = GTK_WIDGET(gtk_widget_get_root(GTK_WIDGET(button)));

    GtkWidget *dialog = adw_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Add data");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent_window));
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 250);

    GtkWidget *toolbar = adw_toolbar_view_new();
    GtkWidget *header = adw_header_bar_new();
    adw_toolbar_view_add_top_bar(ADW_TOOLBAR_VIEW(toolbar), header);

    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(content, 16);
    gtk_widget_set_margin_bottom(content, 16);
    gtk_widget_set_margin_start(content, 16);
    gtk_widget_set_margin_end(content, 16);

    GtkWidget *entry1 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry1), "username");

    GtkWidget *entry2 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry2), "password");

    GtkWidget *addbtn = gtk_button_new_with_label("Add");
    gtk_widget_add_css_class(addbtn, "suggested-action");

    AddData *data = g_new0(AddData, 1);
    data->entry1 = entry1;
    data->entry2 = entry2;
    data->dialog = dialog;
    data->dict = context->dict;
    data->content_box = context->content_box;

    g_signal_connect(addbtn, "clicked", G_CALLBACK(on_add_confirm_clicked), data);
    g_signal_connect(dialog, "destroy", G_CALLBACK(on_dialog_destroy), data);

    gtk_box_append(GTK_BOX(content), entry1);
    gtk_box_append(GTK_BOX(content), entry2);
    gtk_box_append(GTK_BOX(content), addbtn);

    adw_toolbar_view_set_content(ADW_TOOLBAR_VIEW(toolbar), content);
    adw_window_set_content(ADW_WINDOW(dialog), toolbar);

    gtk_window_present(GTK_WINDOW(dialog));
}

void on_dialog_destroy(GtkWidget *widget, gpointer user_data)
{
    AddData *data = (AddData *)user_data;
    if (data->username) {
        g_free(data->username);
    }
    g_free(data);
}

void on_password_window_clicked(GtkButton *button, gpointer user_data)
{
    AddData *context = (AddData *)user_data;
    GtkWidget *parent_window = GTK_WIDGET(gtk_widget_get_root(GTK_WIDGET(button)));

    GtkWidget *dialog = adw_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Set PIN");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent_window));
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 180);

    GtkWidget *toolbar = adw_toolbar_view_new();
    GtkWidget *header = adw_header_bar_new();
    adw_toolbar_view_add_top_bar(ADW_TOOLBAR_VIEW(toolbar), header);

    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(content, 16);
    gtk_widget_set_margin_bottom(content, 16);
    gtk_widget_set_margin_start(content, 16);
    gtk_widget_set_margin_end(content, 16);

    GtkWidget *pass_entry = gtk_password_entry_new();
    gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(pass_entry), TRUE);

    GtkWidget *save_btn = gtk_button_new_with_label("Save Password");
    gtk_widget_add_css_class(save_btn, "suggested-action");

    AddData *data = g_new0(AddData, 1);
    data->entry1 = pass_entry;
    data->dialog = dialog;
    data->dict = context->dict;
    data->master_password_ptr = &context->master_password; 

    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_set_password_confirm), data);

    gtk_box_append(GTK_BOX(content), pass_entry);
    gtk_box_append(GTK_BOX(content), save_btn);

    adw_toolbar_view_set_content(ADW_TOOLBAR_VIEW(toolbar), content);
    adw_window_set_content(ADW_WINDOW(dialog), toolbar);

    gtk_window_present(GTK_WINDOW(dialog));
}

void on_set_password_confirm(GtkButton *button, gpointer user_data)
{
    AddData *data = (AddData *)user_data;
    const char *new_password = gtk_editable_get_text(GTK_EDITABLE(data->entry1));

    if (g_utf8_strlen(new_password, -1) > 0) {
        char *hashed_pass = hash_password(new_password);

        g_file_set_contents("password.txt", hashed_pass, -1, NULL);
        g_free(hashed_pass);

        if (data->master_password_ptr && *data->master_password_ptr) {
            g_free(*data->master_password_ptr);
            *data->master_password_ptr = g_strdup(new_password);

            save_to_json(data->dict, *data->master_password_ptr);
        }

        gtk_window_destroy(GTK_WINDOW(data->dialog));
    }
}

void on_random_clicked(GtkButton *button, gpointer user_data)
{
    AddData *context = (AddData *)user_data;
    GtkWidget *parent_window = GTK_WIDGET(gtk_widget_get_root(GTK_WIDGET(button)));

    GtkWidget *dialog = adw_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Random passwords");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent_window));
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 200);

    GtkWidget *toolbar = adw_toolbar_view_new();
    GtkWidget *header = adw_header_bar_new();
    adw_toolbar_view_add_top_bar(ADW_TOOLBAR_VIEW(toolbar), header);

    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(content, 16);
    gtk_widget_set_margin_bottom(content, 16);
    gtk_widget_set_margin_start(content, 16);
    gtk_widget_set_margin_end(content, 16);

    GtkWidget *label = gtk_label_new("Password length:");

    GtkWidget *scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1.0, 25.0, 1.0);
    gtk_scale_set_draw_value(GTK_SCALE(scale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_TOP);
    gtk_range_set_value(GTK_RANGE(scale), 12.0);

    GtkWidget *gen_btn = gtk_button_new_with_label("Generate");
    gtk_widget_add_css_class(gen_btn, "suggested-action");

    AddData *data = g_new0(AddData, 1);
    data->scale = scale;
    data->dialog = dialog;
    data->dict = context->dict;

    g_signal_connect(gen_btn, "clicked", G_CALLBACK(on_random_confirm_clicked), data);

    gtk_box_append(GTK_BOX(content), label);
    gtk_box_append(GTK_BOX(content), scale);
    gtk_box_append(GTK_BOX(content), gen_btn);

    adw_toolbar_view_set_content(ADW_TOOLBAR_VIEW(toolbar), content);
    adw_window_set_content(ADW_WINDOW(dialog), toolbar);

    gtk_window_present(GTK_WINDOW(dialog));
}

void on_save_clicked(GtkButton *button, gpointer user_data)
{
    AddData *context = (AddData *)user_data;
    save_to_json(context->dict, context->master_password);
}

void on_random_confirm_clicked(GtkButton *button, gpointer user_data)
{
    AddData *data = (AddData *)user_data;
    int pass_len = (int)gtk_range_get_value(GTK_RANGE(data->scale));

    backup_data_file();

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, data->dict);

    char *new_pass = g_malloc(pass_len + 1);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        generate_random_password(new_pass, pass_len);
        g_hash_table_insert(data->dict, g_strdup((const char *)key), g_strdup(new_pass));
    }

    g_free(new_pass);
    gtk_window_destroy(GTK_WINDOW(data->dialog));
}

void on_delete_clicked(GtkButton *button, gpointer user_data)
{
    RowData *data = (RowData *)user_data;
    g_hash_table_remove(data->dict, data->username);
    gtk_box_remove(GTK_BOX(gtk_widget_get_parent(data->row_widget)), data->row_widget);
}

void on_add_confirm_clicked(GtkButton *button, gpointer user_data)
{
    AddData *data = (AddData *)user_data;

    const char *username = gtk_editable_get_text(GTK_EDITABLE(data->entry1));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(data->entry2));

    if (g_utf8_strlen(username, -1) > 0) {
        g_hash_table_insert(data->dict, g_strdup(username), g_strdup(password));

        GtkWidget *parent_window = GTK_WIDGET(gtk_widget_get_root(GTK_WIDGET(button)));
        add_item_to_list(data->content_box, data->dict, username, parent_window);

        gtk_window_destroy(GTK_WINDOW(data->dialog));
    }
}

void on_change_pass_confirm(GtkButton *button, gpointer user_data)
{
    AddData *data = (AddData *)user_data;
    const char *new_pass = gtk_editable_get_text(GTK_EDITABLE(data->entry1));

    if (g_utf8_strlen(new_pass, -1) > 0) {
        g_hash_table_insert(data->dict, g_strdup(data->username), g_strdup(new_pass));
        gtk_window_destroy(GTK_WINDOW(data->dialog));
    }
}

void add_item_to_list(GtkWidget *content_box, GHashTable *dict, const char *username, GtkWidget *parent_window)
{
    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

    GtkWidget *label = gtk_label_new(username);
    gtk_widget_set_hexpand(label, TRUE);
    gtk_widget_set_halign(label, GTK_ALIGN_START);

    GtkWidget *btn_change = gtk_button_new_with_label("Edit");
    GtkWidget *btn_copy = gtk_button_new_with_label("Copy");
    GtkWidget *btn_delete = gtk_button_new_with_label("Delete");
    gtk_widget_add_css_class(btn_delete, "destructive-action");

    RowData *data = g_new0(RowData, 1);
    data->username = g_strdup(username);
    data->dict = dict;
    data->row_widget = row;
    data->parent_window = parent_window;

    g_signal_connect_data(btn_copy, "clicked", G_CALLBACK(on_copy_clicked), data, NULL, 0);
    g_signal_connect_data(btn_change, "clicked", G_CALLBACK(on_change_clicked), data, NULL, 0);
    g_signal_connect_data(btn_delete, "clicked", G_CALLBACK(on_delete_clicked), data, (GClosureNotify)free_row_data, 0);

    gtk_box_append(GTK_BOX(row), label);
    gtk_box_append(GTK_BOX(row), btn_change);
    gtk_box_append(GTK_BOX(row), btn_copy);
    gtk_box_append(GTK_BOX(row), btn_delete);

    gtk_box_append(GTK_BOX(content_box), row);
}

void on_change_clicked(GtkButton *button, gpointer user_data)
{
    RowData *rdata = (RowData *)user_data;

    GtkWidget *dialog = adw_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Change password");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(rdata->parent_window));
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 280, 180);

    GtkWidget *toolbar = adw_toolbar_view_new();
    GtkWidget *header = adw_header_bar_new();
    adw_toolbar_view_add_top_bar(ADW_TOOLBAR_VIEW(toolbar), header);

    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(content, 16);
    gtk_widget_set_margin_bottom(content, 16);
    gtk_widget_set_margin_start(content, 16);
    gtk_widget_set_margin_end(content, 16);

    GtkWidget *entry_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_pass), "New password");

    GtkWidget *save_btn = gtk_button_new_with_label("Save");
    gtk_widget_add_css_class(save_btn, "suggested-action");

    AddData *adata = g_new0(AddData, 1);
    adata->entry1 = entry_pass;
    adata->username = g_strdup(rdata->username);
    adata->dialog = dialog;
    adata->dict = rdata->dict;

    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_change_pass_confirm), adata);

    gtk_box_append(GTK_BOX(content), entry_pass);
    gtk_box_append(GTK_BOX(content), save_btn);

    adw_toolbar_view_set_content(ADW_TOOLBAR_VIEW(toolbar), content);
    adw_window_set_content(ADW_WINDOW(dialog), toolbar);

    gtk_window_present(GTK_WINDOW(dialog));
}
