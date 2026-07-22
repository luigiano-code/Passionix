#include <stdio.h>
#include <string.h>

#include "data.h"

void add()
{
    char scanned_word[32];

    printf("Enter username: ");

    if (fgets(scanned_word, sizeof(scanned_word), stdin) == NULL)
    {
        return;
    }

    scanned_word[strcspn(scanned_word, "\n")] = '\0';

    strcpy(data[data_index].key, scanned_word);


    printf("Enter password: ");

    if (fgets(scanned_word, sizeof(scanned_word), stdin) == NULL)
    {
        return;
    }

    scanned_word[strcspn(scanned_word, "\n")] = '\0';

    strcpy(data[data_index].value, scanned_word);


    data_index++;
}
