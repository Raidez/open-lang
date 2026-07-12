#ifndef AST_H
#define AST_H

#include <stdbool.h>

typedef enum
{
    NODE_IDENTIFIER,           // identifier (variable name)
    NODE_DECLARATION,          // declaration of a variable
    NODE_IMPORTATION,          // importation of a module
    NODE_FUNCTION_DECLARATION, // declaration of a function
    NODE_FUNCTION_CALL,        // call of a function

    // control flow
    NODE_IF,
    NODE_ELSE,
    NODE_MATCH,
    NODE_LOOP,
    NODE_WHILE,
    NODE_FOR,
    NODE_BREAK,
    NODE_CONTINUE,

    NODE_BINARY_OP,  // binary operation
    NODE_LOGICAL_OP, // logical operation

    // literals
    NODE_LITERAL_INT,
    NODE_LITERAL_FLOAT,
    NODE_LITERAL_BOOL,
    NODE_LITERAL_CHAR,
    NODE_LITERAL_STRING,
} NodeType;

typedef enum
{
    TYPE_INFERRED,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_STRING,
} VariableType;

typedef enum {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULO,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_LESS_THAN,
    OP_LESS_THAN_EQUAL,
    OP_GREATER_THAN,
    OP_GREATER_THAN_EQUAL,
    OP_AND,
    OP_OR,
    OP_XOR,
} BinaryOperationType;

typedef enum {
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_LESS_THAN,
    OP_LESS_THAN_EQUAL,
    OP_GREATER_THAN,
    OP_GREATER_THAN_EQUAL,
    OP_AND,
    OP_OR,
    OP_XOR,
} LogicalOperationType;

typedef struct Node Node;

struct Node
{
    NodeType type;
    union
    {
        struct { char *name; } identifier;

        struct {
            char *name;
            bool is_mutable;
            VariableType type;
            Node *value;
        } declaration;

        struct {
            char *module_name;
            Node *imported_modules[];
        } importation;

        struct {
            char **arg_names;
            int arg_count;
            VariableType *arg_types;

            int return_count;
            VariableType *return_types;

            Node *body;
        } function_declaration;

        struct {
            char *function_name;
            int arg_count;
            Node **args;
        } function_call;

        struct {
            BinaryOperationType op;
            Node *left;
            Node *right;
        } binary_op;

        struct {
            LogicalOperationType op;
            Node *left;
            Node *right;
        } logical_op;

        struct { int value; } int_literal;
        struct { float value; } float_literal;
        struct { bool value; } boolean_literal;
        struct { char value; } char_literal;
        struct { char *value; } string_literal;
    };
};



Node *node_identifier(char *name);
Node *node_declaration(char *name, bool is_mutable, VariableType type, Node *value);
Node *node_importation(char *module_name, Node *imported_modules[]);
Node *node_function_declaration(char **arg_names, int arg_count, VariableType *arg_types, int return_count, VariableType *return_types, Node *body);
Node *node_function_call(char *function_name, int arg_count, Node **args);

Node *node_binary_op(BinaryOperationType op, Node *left, Node *right);
Node *node_logical_op(LogicalOperationType op, Node *left, Node *right);

Node *node_int_literal(int value);
Node *node_float_literal(float value);
Node *node_boolean_literal(bool value);
Node *node_char_literal(char value);
Node *node_string_literal(char *value);

void free_node(Node *node);

#endif
