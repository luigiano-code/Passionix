#ifndef CRYPTO_H
#define CRYPTO_H

#include <glib.h>

char *encrypt_string(const char *plaintext, const char *password);

char *decrypt_string(const char *json_input, const char *password);

#endif
