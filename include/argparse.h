#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <stdbool.h>

typedef enum
{
    POSITIONAL,
    OPTIONAL,
    FLAG,
} ArgType;

typedef struct
{
    char *name;
    ArgType type;
    char *description;
    char *default_value;
    char *value;
} Argument;

/**
 * Get the value of a command-line argument by its name.
 * @param name The name of the argument.
 * @return The value of the argument, or NULL if not found.
 */
char *get_argument_value(char *name);

/**
 * Check if a flag is set in the command-line arguments.
 * @param name The name of the flag.
 * @return true if the flag is set, false otherwise.
 */
bool is_flag_set(char *name);

/**
 * Parse command-line arguments.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 */
void argparse(int argc, char *argv[]);

#endif
