#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

typedef enum
{
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_STRING,
    VALUE_BOOL,
} ValueType;

typedef struct
{
    ValueType type;
    union
    {
        int int_value;
        float float_value;
        char *string_value;
        int bool_value;
    };
} Value;

/**
 * Interpret the given AST node for debug
 */
Value interpret(Node *node);

#endif
