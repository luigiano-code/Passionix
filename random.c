#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "user.h"
#include "data.h"
#include "save.h"

void generate_password(char *buffer, int length)
{
    const char charset[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%^&*()-_=+";

    int charset_size = sizeof(charset) - 1;

    for (int i = 0; i < length; i++)
    {
        buffer[i] = charset[rand() % charset_size];
    }

    buffer[length] = '\0';
}

void random_cli(void)
{
    printf("Password length: ");
    int length = get_choice();

    srand(time(NULL));

	save_data("old.json");

    for (int i = 0; i < data_index; i++)
    {
        generate_password(data[i].value, length);
    }
}


