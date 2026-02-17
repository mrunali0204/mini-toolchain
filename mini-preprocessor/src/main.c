#include <stdio.h>
#include <stdlib.h>
#include "reader.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Using: mini-cpp <file.c\n");
        return 1;
    }

    char *source = read_file(argv[1]);
    if(!source)
    {
        fprintf(stderr, "Failed to read file\n");
        return 1;
    }

    printf("%s", source);

    free(source);
    return 0;

}

