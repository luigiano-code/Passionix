#include "password-operations.h"
#include <stdlib.h> 
#include <glib.h>   

char *hash_password(const char *password)
{
    if (!password) return NULL;
    return g_compute_checksum_for_string(G_CHECKSUM_SHA256, password, -1);
}

void generate_random_password(char *dest, int length)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyz"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "0123456789"
                           "!@#$%^&*()_+-=[]{}|;:,.<>?";
    int charset_size = sizeof(charset) - 1;

    for (int i = 0; i < length; i++) {
        int key = rand() % charset_size;
        dest[i] = charset[key];
    }
    dest[length] = '\0';
}
