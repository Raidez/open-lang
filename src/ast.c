#include <stdlib.h>
#include "ast.h"

Node *node_int_literal(int value)
{
    Node *node = malloc(sizeof(Node));
    node->type = NODE_LITERAL_INT;
    node->int_literal.value = value;
    return node;
}

Node *node_float_literal(float value)
{
    Node *node = malloc(sizeof(Node));
    node->type = NODE_LITERAL_FLOAT;
    node->float_literal.value = value;
    return node;
}

Node *node_boolean_literal(bool value)
{
    Node *node = malloc(sizeof(Node));
    node->type = NODE_LITERAL_BOOL;
    node->boolean_literal.value = value;
    return node;
}

Node *node_char_literal(char value)
{
    Node *node = malloc(sizeof(Node));
    node->type = NODE_LITERAL_CHAR;
    node->char_literal.value = value;
    return node;
}

Node *node_string_literal(char *value)
{
    Node *node = malloc(sizeof(Node));
    node->type = NODE_LITERAL_STRING;
    node->string_literal.value = value;
    return node;
}

Node *node_identifier(char *name)
{
    Node *node = malloc(sizeof(Node));
    node->type = NODE_IDENTIFIER;
    node->identifier.name = name;
    return node;
}

Node *node_declaration(char *name, bool is_mutable, VariableType type, Node *value)
{
    Node *node = malloc(sizeof(Node));
    node->type = NODE_DECLARATION;
    node->declaration.name = name;
    node->declaration.is_mutable = is_mutable;
    node->declaration.type = type;
    node->declaration.value = value;
    return node;
}

Node *node_binary_op(OperationType op, Node *left, Node *right)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_BINARY_OP;
    node->binary_op.op = op;
    node->binary_op.left = left;
    node->binary_op.right = right;
    return node;
}

void free_node(Node *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
    case NODE_DECLARATION:
        free(node->declaration.name);
        free_node(node->declaration.value);
        break;
    case NODE_BINARY_OP:
        free_node(node->binary_op.left);
        free_node(node->binary_op.right);
        break;
    case NODE_LITERAL_STRING:
        free(node->string_literal.value);
        break;
    case NODE_IDENTIFIER:
        free(node->identifier.name);
        break;
    default:
        break;
    }

    free(node);
}
