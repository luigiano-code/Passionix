#include <stdio.h>

#include "data.h"

int data_index = 0;
Dictionary data[128];

void list_data()
{
	for (int i = 0; i < data_index; i++)
	{
		printf(data[i].key);
		printf(" | ");
		printf(data[i].value);
	}
}
