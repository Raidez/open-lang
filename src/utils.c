#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

char *read_file(char *input_file)
{
    // Open file
    FILE *file = fopen(input_file, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Can't open file '%s'\n", input_file);
        return NULL;
    }

    // Get the size of the file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Allocate a buffer to hold the file contents
    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    // Close the file after reading
    fclose(file);

    return buffer;
}
