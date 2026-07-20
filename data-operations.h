#ifndef DATA_OPERATIONS_H
#define DATA_OPERATIONS_H

#include <glib.h>
#include <adwaita.h>

#define DATA_FILE "data.json"
#define BACKUP_FILE "old.json"

void free_row_data(gpointer user_data);
void backup_data_file(void);
void save_to_json(GHashTable *dict, const char *master_password);
void load_from_json(GtkWidget *content_box, GHashTable *dict, GtkWidget *parent_window, const char *master_password);

#endif
