#include <stdio.h>

#include "std.h"

static void print_value(Value value)
{
    switch (value.type)
    {
    case TYPE_INT:
        printf("%d", value.value.int_value);
        break;

    case TYPE_FLOAT:
        printf("%f", value.value.float_value);
        break;

    case TYPE_BOOL:
        printf("%s", value.value.bool_value ? "true" : "false");
        break;

    case TYPE_CHAR:
        printf("%c", value.value.char_value);
        break;

    case TYPE_STRING:
        printf("%s", value.value.string_value);
        break;

    default:
        break;
    }
}

void print(Tuple args)
{
    for (size_t i = 0; i < args.size; i++)
    {
        print_value(args.items[i]);
        if (i + 1 < args.size)
            printf(" ");
    }
}

void println(Tuple args)
{
    for (size_t i = 0; i < args.size; i++)
    {
        print_value(args.items[i]);
        if (i + 1 < args.size)
            printf(" ");
    }
    printf("\n");
}
