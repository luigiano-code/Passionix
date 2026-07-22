#include <stdio.h>
#include <string.h>

#include "login.h"
#include "data.h"
#include "user.h"

int login()
{
	char password[32];
	
	printf("password: ");
	if (check_login( "abc"))
	{
		list_data();
		printf("options:\n1. Add new\n");
		printf("select: ");
		if (get_choice() == 1)
		{
			return 1;
		} 
		else
		{
			return 0;
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

    if (fgets(scanned_password, sizeof(scanned_password), stdin) == NULL)
    {
        return false;
    }

    scanned_password[strcspn(scanned_password, "\n")] = '\0';

    if (strcmp(scanned_password, password) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
