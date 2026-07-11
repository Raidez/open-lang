#ifndef AST_H
#define AST_H

#include <stdbool.h>

typedef enum
{
    NODE_DECLARATION,
} NodeType;

typedef enum
{
    INTEGER,
    FLOAT,
    STRING,
} VariableType;

typedef struct
{
    char *name;
    VariableType type;
    bool is_mutable;
    union
    {
        int int_value;
        float float_value;
        char *string_value;
    } value;
} VariableDeclaration;

typedef struct
{
    NodeType type;
    union
    {
        VariableDeclaration variable_declaration;
    } value;
} Node;

#endif
