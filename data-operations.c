#include "data-operations.h"
#include "crypto.h"
#include <json-glib/json-glib.h>
#include "app.h"
#include "user-operations.h"
#include "clipboard-operations.h"

void free_row_data(gpointer user_data)
{
    RowData *data = (RowData *)user_data;
    g_free(data->username);
    g_free(data);
}

void backup_data_file(void)
{
    if (!g_file_test(DATA_FILE, G_FILE_TEST_EXISTS)) {
        return;
    }

    gchar *content = NULL;
    gsize length = 0;
    GError *error = NULL;

    if (g_file_get_contents(DATA_FILE, &content, &length, &error)) {
        g_file_set_contents(BACKUP_FILE, content, length, NULL);
        g_free(content);
    } else {
        if (error) {
            g_warning("Backup creation failed: %s", error->message);
            g_clear_error(&error);
        }
    }
}

void load_from_json(GtkWidget *content_box, GHashTable *dict, GtkWidget *parent_window, const char *master_password)
{
    if (!g_file_test(DATA_FILE, G_FILE_TEST_EXISTS)) return;

    gchar *encrypted_content = NULL;
    if (!g_file_get_contents(DATA_FILE, &encrypted_content, NULL, NULL)) return;

    char *decrypted_json = decrypt_string(encrypted_content, master_password);
    g_free(encrypted_content);

    if (!decrypted_json) {
        g_warning("Błąd odszyfrowania pliku! Niepoprawne hasło klucza lub uszkodzony plik.");
        return;
    }

    JsonParser *parser = json_parser_new();
    if (json_parser_load_from_data(parser, decrypted_json, -1, NULL)) {
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
    }

    g_free(decrypted_json);
    g_object_unref(parser);
}

void save_to_json(GHashTable *dict, const char *master_password)
{
    if (!master_password) {
        g_warning("Brak klucza do zaszyfrowania bazy!");
        return;
    }

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

    char *raw_json = json_generator_to_data(generator, NULL);

    char *encrypted_json = encrypt_string(raw_json, master_password);

    if (encrypted_json) {
        g_file_set_contents(DATA_FILE, encrypted_json, -1, NULL);
        g_free(encrypted_json);
    }

    g_free(raw_json);
    g_object_unref(generator);
    json_node_free(root);
    g_object_unref(builder);
}
