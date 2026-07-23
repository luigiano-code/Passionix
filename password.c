#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "data.h"
#include "password.h"


void hash_password(const char *password, char *output)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256((const unsigned char *)password, strlen(password), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
        sprintf(output + (i * 2), "%02x", hash[i]);
	}
    output[64] = '\0';
}

void load_password(void)
{
    FILE *password_txt = fopen("password.txt", "r");
    if (password_txt == NULL)
    {
        main_password = NULL;
        return;
    }

    char buff[65];

    if (fgets(buff, sizeof(buff), password_txt) != NULL)
    {
        buff[strcspn(buff, "\n")] = '\0';
        main_password = strdup(buff);
    }
    else
    {
        main_password = NULL;
    }

    fclose(password_txt);
}

void password()
{
	char scanned_word[32];

	printf("Set new password: ");
	
	fgets(scanned_word, sizeof(scanned_word), stdin);
	scanned_word[strcspn(scanned_word, "\n")] = '\0';
	main_password = strdup(scanned_word);
	
	char hash[65];

    hash_password(main_password, hash);

    FILE *f = fopen("password.txt", "w");
    if (f)
    {
        fprintf(f, "%s\n", hash);
        fclose(f);
    }

}
