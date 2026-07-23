#include <stdio.h>

#include "save.h"
#include "data.h"

void compare()
{
	load_data("old.json");
	printf("--- Old data ---\n");
	list_data();
	load_data("data.json");
	printf("--- Current data ---\n");
	list_data();
}
