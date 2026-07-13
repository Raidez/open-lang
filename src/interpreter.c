#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "parser.h"

typedef struct
{
    char *name;
    Value value;
} Symbol;

static Value interpret_operation(Node *node, Value left, Value right)
{
    switch (node->binary_op.op)
    {
    case OP_ADD:
    case OP_ADD_ASSIGN:
        return (Value){VALUE_INT, .int_value = left.int_value + right.int_value};
    case OP_SUBTRACT:
    case OP_SUBTRACT_ASSIGN:
        return (Value){VALUE_INT, .int_value = left.int_value - right.int_value};
    case OP_MULTIPLY:
    case OP_MULTIPLY_ASSIGN:
        return (Value){VALUE_INT, .int_value = left.int_value * right.int_value};
    case OP_DIVIDE:
    case OP_DIVIDE_ASSIGN:
        return (Value){VALUE_INT, .int_value = left.int_value / right.int_value};
    case OP_MODULO:
    case OP_MODULO_ASSIGN:
        return (Value){VALUE_INT, .int_value = left.int_value % right.int_value};
    default:
        fprintf(stderr, "Unknown operation type %d\n", node->binary_op.op);
        exit(1);
    }
}

Symbol symbols[256];
int symbol_count = 0;

Value interpret(Node *node)
{
    switch (node->type)
    {
    case NODE_BLOCK:
    {
        Value last_value = {0};
        for (int i = 0; i < node->block.count; i++)
        {
            last_value = interpret(node->block.statements[i]);
        }
        return last_value;
    }
    case NODE_LITERAL:
    {
        switch (node->literal.type)
        {
        case TYPE_INT:
            return (Value){.type = VALUE_INT, .int_value = node->literal.int_value};
            break;
        case TYPE_FLOAT:
            return (Value){.type = VALUE_FLOAT, .float_value = node->literal.float_value};
            break;
        case TYPE_BOOL:
            return (Value){.type = VALUE_BOOL, .bool_value = node->literal.bool_value};
            break;
        case TYPE_STRING:
            return (Value){.type = VALUE_STRING, .string_value = node->literal.string_value};
            break;
        default:
            fprintf(stderr, "Unknown literal type %d\n", node->literal.type);
            exit(1);
        }
    }
    case NODE_BINARY_OP:
    {
        Value left = interpret(node->binary_op.left);
        Value right = interpret(node->binary_op.right);
        return interpret_operation(node, left, right);
    }
    case NODE_DECLARATION:
    {
        Value value = interpret(node->declaration.value);
        symbols[symbol_count++] = (Symbol){.name = node->declaration.name, .value = value};
        printf("%s = %d\n", node->declaration.name, value.int_value);
        return value;
    }
    case NODE_IDENTIFIER:
    {
        for (int i = 0; i < symbol_count; i++)
        {
            if (strcmp(symbols[i].name, node->identifier.name) == 0)
            {
                return symbols[i].value;
            }
        }
        fprintf(stderr, "Undefined variable '%s'\n", node->identifier.name);
        exit(1);
    }
    default:
        fprintf(stderr, "Unknown node type %s\n",
                node_type_to_string(node->type));
        exit(1);
    }
}
