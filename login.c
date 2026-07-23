#include <stdio.h>
#include <string.h>

#include "login.h"
#include "data.h"
#include "user.h"
#include "save.h"
#include "password.h"

int login()
{
	char password[32];
	
	load_password();

	printf("password: ");
	printf("%s", main_password );
	printf("\n---\n");
	if (check_login( main_password ))
	{
		load_data("data.json");
		list_data();
		printf("options:\n1. Add new\n2. Random\n3. Password\n4. Compare\n");
		printf("select: ");
		int choice = get_choice();
		if ( choice == 1)
		{
			return 1;
		} 
		else if ( choice == 2 )
		{
			return 2;
		}
		else if ( choice == 3 )
		{
			return 3;
		}
		else if ( choice == 4 )
		{
			return 4;
		}
	}
	else
	{
		return 0;
	}
}

bool check_login(const char *password)
{
    char scanned_password[32];
	char hash[65];

    if (fgets(scanned_password, sizeof(scanned_password), stdin) == NULL)
    {
        return false;
    }

    scanned_password[strcspn(scanned_password, "\n")] = '\0';

	hash_password(scanned_password, hash);

	printf("%s", hash);

    if (strcmp(hash, password) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
