#include "app.h"
#include <json-glib/json-glib.h>

#define DATA_FILE "data.json"

typedef struct {
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *dialog;
    GHashTable *dict;
    GtkWidget *content_box;
    char *username;
} AddData;

typedef struct {
    char *username;
    GHashTable *dict;
    GtkWidget *row_widget;
    GtkWidget *parent_window;
} RowData;

static void add_item_to_list(GtkWidget *content_box, GHashTable *dict, const char *username, GtkWidget *parent_window);

static void save_to_json(GHashTable *dict)
{
    JsonBuilder *builder = json_builder_new();
    json_builder_begin_object(builder);

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, dict);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        json_builder_set_member_name(builder, (const char *)key);
        json_builder_add_string_value(builder, (const char *)value);
    }

    json_builder_end_object(builder);

    JsonGenerator *generator = json_generator_new();
    JsonNode *root = json_builder_get_root(builder);
    json_generator_set_root(generator, root);
    json_generator_set_pretty(generator, TRUE);

    GError *error = NULL;
    if (!json_generator_to_file(generator, DATA_FILE, &error)) {
        g_warning("File save error: %s", error->message);
        g_clear_error(&error);
    }

    g_object_unref(generator);
    json_node_free(root);
    g_object_unref(builder);
}

static void load_from_json(GtkWidget *content_box, GHashTable *dict, GtkWidget *parent_window)
{
    if (!g_file_test(DATA_FILE, G_FILE_TEST_EXISTS)) {
        return;
    }

    JsonParser *parser = json_parser_new();
    GError *error = NULL;

    if (!json_parser_load_from_file(parser, DATA_FILE, &error)) {
        g_warning("JSON file load error: %s", error->message);
        g_clear_error(&error);
        g_object_unref(parser);
        return;
    }

    JsonNode *root = json_parser_get_root(parser);
    if (JSON_NODE_HOLDS_OBJECT(root)) {
        JsonObject *obj = json_node_get_object(root);
        GList *members = json_object_get_members(obj);

        for (GList *l = members; l != NULL; l = l->next) {
            const char *username = (const char *)l->data;
            const char *password = json_object_get_string_member(obj, username);

            if (username && password) {
                g_hash_table_insert(dict, g_strdup(username), g_strdup(password));
                add_item_to_list(content_box, dict, username, parent_window);
            }
        }
        g_list_free(members);
    }

    g_object_unref(parser);
}

static void copy_to_clipboard(GtkWidget *widget, const char *text)
{
    GdkDisplay *display = gtk_widget_get_display(widget);
    GdkClipboard *clipboard = gdk_display_get_clipboard(display);
    gdk_clipboard_set_text(clipboard, text);
}

static void on_copy_clicked(GtkButton *button, gpointer user_data)
{
    RowData *data = (RowData *)user_data;
    const char *password = g_hash_table_lookup(data->dict, data->username);
    if (password) {
        copy_to_clipboard(GTK_WIDGET(button), password);
    }
}

static void on_delete_clicked(GtkButton *button, gpointer user_data)
{
    RowData *data = (RowData *)user_data;
    g_hash_table_remove(data->dict, data->username);
    gtk_box_remove(GTK_BOX(gtk_widget_get_parent(data->row_widget)), data->row_widget);
}

static void on_change_pass_confirm(GtkButton *button, gpointer user_data)
{
    AddData *data = (AddData *)user_data;
    const char *new_pass = gtk_editable_get_text(GTK_EDITABLE(data->entry1));

    if (g_utf8_strlen(new_pass, -1) > 0) {
        g_hash_table_insert(data->dict, g_strdup(data->username), g_strdup(new_pass));
        gtk_window_destroy(GTK_WINDOW(data->dialog));
    }
}

static void on_change_clicked(GtkButton *button, gpointer user_data)
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

static void free_row_data(gpointer user_data)
{
    RowData *data = (RowData *)user_data;
    g_free(data->username);
    g_free(data);
}

static void add_item_to_list(GtkWidget *content_box, GHashTable *dict, const char *username, GtkWidget *parent_window)
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

static void on_add_confirm_clicked(GtkButton *button, gpointer user_data)
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

static void on_save_clicked(GtkButton *button, gpointer user_data)
{
    AddData *context = (AddData *)user_data;
    save_to_json(context->dict);
}

static void on_dialog_destroy(GtkWidget *widget, gpointer user_data)
{
    AddData *data = (AddData *)user_data;
    if (data->username) {
        g_free(data->username);
    }
    g_free(data);
}

static void on_add_clicked(GtkButton *button, gpointer user_data)
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

void show_main_view(AdwApplication *app)
{
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

    const char *labels[5] = {"S", "C", "P", "R", "+"};
    for (int i = 0; i < 5; i++) {
        GtkWidget *btn = gtk_button_new_with_label(labels[i]);

        if (g_strcmp0(labels[i], "+") == 0) {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_add_clicked), context);
        } else if (g_strcmp0(labels[i], "S") == 0) {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_save_clicked), context);
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

    load_from_json(content, dict, window);

    gtk_window_present(GTK_WINDOW(window));
}
