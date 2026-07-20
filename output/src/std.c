#include <stdio.h>

#include "std.h"

static void print_argument(Value arg)
{
    switch (arg.type)
    {
    case TYPE_INT:
        printf("%d", arg.value.int_value);
        break;

    case TYPE_FLOAT:
        printf("%f", arg.value.float_value);
        break;

    case TYPE_BOOL:
        printf("%s", arg.value.bool_value ? "true" : "false");
        break;

    case TYPE_CHAR:
        printf("%c", arg.value.char_value);
        break;

    case TYPE_STRING:
        printf("%s", arg.value.string_value);
        break;

    default:
        break;
    }
}

void print(Tuple args)
{
    for (size_t i = 0; i < args.size; i++)
    {
        print_argument(args.items[i]);
        if (i + 1 < args.size)
            printf(" ");
    }
}

void println(Tuple args)
{
    for (size_t i = 0; i < args.size; i++)
    {
        print_argument(args.items[i]);
        if (i + 1 < args.size)
            printf(" ");
    }
    printf("\n");
}
