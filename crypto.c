#include "crypto.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <json-glib/json-glib.h>
#include <string.h>

#define SALT_LEN 16
#define IV_LEN 16
#define KEY_LEN 32
#define PBKDF2_ITERATIONS 10000

static gboolean derive_key(const char *password, const unsigned char *salt, unsigned char *key_out)
{
    return PKCS5_PBKDF2_HMAC(
        password, (int)strlen(password),
        salt, SALT_LEN,
        PBKDF2_ITERATIONS,
        EVP_sha256(),
        KEY_LEN, key_out
    ) == 1;
}

char *encrypt_string(const char *plaintext, const char *password)
{
    if (!plaintext || !password) return NULL;

    unsigned char salt[SALT_LEN];
    unsigned char iv[IV_LEN];
    unsigned char key[KEY_LEN];

    if (RAND_bytes(salt, SALT_LEN) != 1 || RAND_bytes(iv, IV_LEN) != 1) {
        return NULL;
    }

    if (!derive_key(password, salt, key)) {
        return NULL;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0, ciphertext_len = 0;
    int plain_len = (int)strlen(plaintext);
    
    unsigned char *ciphertext = g_malloc(plain_len + EVP_MAX_BLOCK_LENGTH);

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext, &len, (const unsigned char *)plaintext, plain_len);
    ciphertext_len = len;
    
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    gchar *b64_salt = g_base64_encode(salt, SALT_LEN);
    gchar *b64_iv = g_base64_encode(iv, IV_LEN);
    gchar *b64_data = g_base64_encode(ciphertext, ciphertext_len);

    g_free(ciphertext);

    JsonBuilder *builder = json_builder_new();
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "salt");
    json_builder_add_string_value(builder, b64_salt);
    json_builder_set_member_name(builder, "iv");
    json_builder_add_string_value(builder, b64_iv);
    json_builder_set_member_name(builder, "data");
    json_builder_add_string_value(builder, b64_data);
    json_builder_end_object(builder);

    JsonGenerator *gen = json_generator_new();
    JsonNode *root = json_builder_get_root(builder);
    json_generator_set_root(gen, root);
    
    char *result_json = json_generator_to_data(gen, NULL);

    g_free(b64_salt);
    g_free(b64_iv);
    g_free(b64_data);
    json_node_free(root);
    g_object_unref(builder);
    g_object_unref(gen);

    return result_json;
}

char *decrypt_string(const char *json_input, const char *password)
{
    if (!json_input || !password) return NULL;

    JsonParser *parser = json_parser_new();
    if (!json_parser_load_from_data(parser, json_input, -1, NULL)) {
        g_object_unref(parser);
        return NULL;
    }

    JsonObject *obj = json_node_get_object(json_parser_get_root(parser));
    const char *b64_salt = json_object_get_string_member(obj, "salt");
    const char *b64_iv = json_object_get_string_member(obj, "iv");
    const char *b64_data = json_object_get_string_member(obj, "data");

    gsize salt_len, iv_len, data_len;
    guchar *salt = g_base64_decode(b64_salt, &salt_len);
    guchar *iv = g_base64_decode(b64_iv, &iv_len);
    guchar *ciphertext = g_base64_decode(b64_data, &data_len);

    unsigned char key[KEY_LEN];
    derive_key(password, salt, key);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0, plaintext_len = 0;
    unsigned char *plaintext = g_malloc(data_len + EVP_MAX_BLOCK_LENGTH);

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, (int)data_len);
    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) <= 0) {
        g_free(plaintext);
        plaintext = NULL;
    } else {
        plaintext_len += len;
        plaintext[plaintext_len] = '\0';
    }

    EVP_CIPHER_CTX_free(ctx);
    g_free(salt);
    g_free(iv);
    g_free(ciphertext);
    g_object_unref(parser);

    return (char *)plaintext;
}
