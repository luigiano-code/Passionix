#include <stdio.h>
#include <string.h>

#include "data.h"

void add(char *username, char *password)
{
    strcpy(data[data_index].key, username);

    strcpy(data[data_index].value, password);

    data_index++;
}
