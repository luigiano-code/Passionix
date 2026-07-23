#include <json-glib/json-glib.h>
#include <stdio.h>

#include "data.h"

void save_data(char *data_file)
{
    JsonBuilder *builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "passwords");
    json_builder_begin_array(builder);

    for (int i = 0; i < data_index; i++)
    {
        json_builder_begin_object(builder);

        json_builder_set_member_name(builder, "username");
        json_builder_add_string_value(builder, data[i].key);

        json_builder_set_member_name(builder, "password");
        json_builder_add_string_value(builder, data[i].value);

        json_builder_end_object(builder);
    }

    json_builder_end_array(builder);
    json_builder_end_object(builder);

    JsonGenerator *generator = json_generator_new();
    JsonNode *root = json_builder_get_root(builder);

    json_generator_set_root(generator, root);
    json_generator_set_pretty(generator, TRUE);

    if (!json_generator_to_file(generator, data_file, NULL))
    {
        printf("Cannot save json\n");
    }

    json_node_free(root);
    g_object_unref(generator);
    g_object_unref(builder);
}

void load_data(char *data_file)
{
    JsonParser *parser = json_parser_new();

    if (!json_parser_load_from_file(parser, data_file, NULL))
    {
        g_object_unref(parser);
        return;
    }

    data_index = 0;

    JsonNode *root = json_parser_get_root(parser);
    JsonObject *object = json_node_get_object(root);

    JsonArray *passwords =
        json_object_get_array_member(object, "passwords");

    if (passwords == NULL)
    {
        g_object_unref(parser);
        return;
    }

    guint length = json_array_get_length(passwords);

    for (guint i = 0; i < length && i < 128; i++)
    {
        JsonObject *entry =
            json_array_get_object_element(passwords, i);

        const char *username =
            json_object_get_string_member(entry, "username");

        const char *password =
            json_object_get_string_member(entry, "password");

        strncpy(data[data_index].key, username,
                sizeof(data[data_index].key) - 1);
        data[data_index].key[sizeof(data[data_index].key) - 1] = '\0';

        strncpy(data[data_index].value, password,
                sizeof(data[data_index].value) - 1);
        data[data_index].value[sizeof(data[data_index].value) - 1] = '\0';

        data_index++;
    }

    g_object_unref(parser);
}
