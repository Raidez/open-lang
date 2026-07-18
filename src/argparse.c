#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "argparse.h"

/**** STATIC ****/

static void print_usage()
{
    printf("Usage: open <input_file> [--lexer] [--ast] [--interpreter] [--symbols]\n");
    printf("Options:\n");
    printf("  --lexer        Show lexer output.\n");
    printf("  --ast          Show AST output.\n");
    printf("  --symbols      Show symbol table.\n");
    printf("  --interpreter  Run the interpreter.\n");
}

static Argument arguments[] = {
    {"input_file", POSITIONAL, "The input file to process.", NULL, NULL},
    {"--lexer", FLAG, "Show lexer output.", "false", NULL},
    {"--ast", FLAG, "Show AST output.", "false", NULL},
    {"--symbols", FLAG, "Show symbol table.", "false", NULL},
    {"--interpreter", FLAG, "Run the interpreter.", "false", NULL},
};
static int argument_count = sizeof(arguments) / sizeof(arguments[0]);

/**** PUBLIC ****/

char *get_argument_value(char *name)
{
    for (int i = 0; i < argument_count; i++)
    {
        if (strcmp(arguments[i].name, name) == 0)
            return arguments[i].value;
    }
    return NULL;
}

bool is_flag_set(char *name)
{
    char *value = get_argument_value(name);
    return value != NULL && strcmp(value, "true") == 0;
}

void argparse(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_usage();
        exit(0);
    }

    for (int i = 1; i < argc; i++)
    {
        char *arg = argv[i];

        for (int j = 0; j < argument_count; j++)
        {
            Argument *argument = &arguments[j];

            // Handle positional arguments
            if (argument->type == POSITIONAL && argument->value == NULL)
            {
                argument->value = argv[i];
                break;
            }

            // Handle optional arguments (flags and options)
            if (strcmp(arg, argument->name) != 0)
                continue;

            if (argument->type == FLAG)
            {
                argument->value = "true";
                break;
            }
        }
    }
}
