#ifndef DATA_H
#define DATA_H

#include <gtk/gtk.h>
#include <adwaita.h>

typedef struct
{
    char key[32];
    char value[32];
} Dictionary;

extern int data_index;
extern Dictionary data[128];
extern char* main_password;

void list_data(GtkWidget *scroll_box, int readonly_mode);

#endif
