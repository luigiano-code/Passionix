#ifndef PASSWORD_OPERATIONS_H
#define PASSWORD_OPERATIONS_H

#include <glib.h>

char *hash_password(const char *password);
void generate_random_password(char *dest, int length);

#endif
