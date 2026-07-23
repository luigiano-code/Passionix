#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "password.h"
#include "add.h"
#include "random.h"
#include "data.h"
#include "random.h"
#include "login.h"
#include "save.h"
#include "compare.h"

int main()
{

	int action = login();
	int c;

	srand(time(NULL));


	if ( action == 1 )
	{
		while ((c = getchar()) != '\n' && c != EOF);
		add();		
		save_data("data.json");
	}
	else if	( action == 2 )
	{
		while ((c = getchar()) != '\n' && c != EOF);
		random_cli();
		save_data("data.json");
	}
	else if ( action == 3 )
	{
		while ((c = getchar()) != '\n' && c != EOF);
		password();
	}
	else if ( action == 4 )
	{
		compare();
	}
	else if ( action == 5 )
	{
		printf("save");
	}
}

