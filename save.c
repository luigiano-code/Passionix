#include <stdio.h>
#include "data.h"

void save_data()
{
    FILE *file = fopen("data.json", "w");

    if (file == NULL)
    {
        printf("Cannot open file\n");
        return;
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"passwords\": [\n");

    for (int i = 0; i < data_index; i++)
    {
        fprintf(file,
            "    {\n"
            "      \"username\": \"%s\",\n"
            "      \"password\": \"%s\"\n"
            "    }%s\n",
            data[i].key,
            data[i].value,
            i == data_index - 1 ? "" : ","
        );
    }

    fprintf(file, "  ]\n");
    fprintf(file, "}\n");

    fclose(file);
}
