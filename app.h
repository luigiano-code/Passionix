#ifndef APP_H
#define APP_H

#include <adwaita.h>

char *hash_password(const char *password);
void show_main_view(AdwApplication *app, const char *master_password);

typedef struct {
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *dialog;
    GHashTable *dict;
    GtkWidget *content_box;
    char *username;
    GtkWidget *scale;
    char *master_password;        // NAPRAWIONE: Dodano brakujące pole
    char **master_password_ptr;
} AddData;

typedef struct {
    char *username;
    GHashTable *dict;
    GtkWidget *row_widget;
    GtkWidget *parent_window;
} RowData;

#endif
