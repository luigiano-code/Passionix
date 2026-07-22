#include <stdio.h>

#include "add.h"
#include "data.h"
#include "login.h"
#include "save.h"

int main()
{

	int action = login();

	if ( action == 1 )
	{
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
		add();		
		save_data();
	}
	else if	( action == 2 )
	{
		printf("random");
	}
	else if ( action == 3 )
	{
		printf("change password");
	}
	else if ( action == 4 )
	{
		printf("compare");
	}
	else if ( action == 5 )
	{
		printf("save");
	}
}

