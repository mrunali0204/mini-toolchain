#include <stdio.h>
#include <stdlib.h>
#include "reader.h"

char *read_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("fopen");
        return NULL;
    }


    fseek (fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = malloc(size + 1);
    if(!buffer)
    {
        fclose(fp);
        return NULL;
    }

    fread(buffer, 1, size, fp);
    buffer[size] = '\0';

    fclose(fp);
    return buffer;

}