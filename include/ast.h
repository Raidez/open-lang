#ifndef AST_H
#define AST_H

#include <stdbool.h>

typedef enum
{
    NODE_DECLARATION,
    NODE_BINARY_OP,
    NODE_LITERAL_INT,
    NODE_LITERAL_FLOAT,
    NODE_LITERAL_STRING,
    NODE_LITERAL_BOOL,
    NODE_IDENTIFIER,
} NodeType;

typedef enum
{
    TYPE_INFERRED,
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOLEAN,
} VariableType;

typedef enum {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULO,
} OperationType;

typedef struct Node Node;

struct Node
{
    NodeType type;
    union
    {
        struct {
            char *name;
            bool is_mutable;
            VariableType type;
            Node *value;
        } declaration;

        struct {
            OperationType op;
            Node *left;
            Node *right;
        } binary_op;

        struct { int value; } int_literal;
        struct { float value; } float_literal;
        struct { char *value; } string_literal;
        struct { bool value; } boolean_literal;

        struct { char *name; } identifier;
    };
};

Node *node_int_literal(int value);
Node *node_float_literal(float value);
Node *node_string_literal(char *value);
Node *node_boolean_literal(bool value);
Node *node_identifier(char *name);
Node *node_declaration(char *name, bool is_mutable, VariableType type, Node *value);
Node *node_binary_op(OperationType op, Node *left, Node *right);

void free_node(Node *node);

#endif
