#ifndef DATA_H
#define DATA_H

typedef struct
{
    char key[32];
    char value[32];
} Dictionary;

extern int data_index;
extern Dictionary data[128];

void list_data();

#endif
