#ifndef AST_H
#define AST_H

#include <stdbool.h>

typedef enum
{
    NODE_BLOCK,

    NODE_IDENTIFIER,           // identifier (variable name)
    NODE_DECLARATION,          // declaration of a variable
    NODE_IMPORTATION,          // importation of a module
    NODE_FUNCTION_DECLARATION, // declaration of a function
    NODE_FUNCTION_CALL,        // call of a function

    // control flow
    NODE_FLOW, // if, else, match
    NODE_LOOP, // while, for, loop
    NODE_STOP, // return, break, continue

    NODE_UNARY_OP,   // unary operation
    NODE_BINARY_OP,  // binary operation
    NODE_LOGICAL_OP, // logical operation

    // literals
    NODE_LITERAL,
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

typedef enum
{
    OP_NOT,
} UnaryOperationType;

typedef enum
{
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULO,

    OP_ADD_ASSIGN,
    OP_SUBTRACT_ASSIGN,
    OP_MULTIPLY_ASSIGN,
    OP_DIVIDE_ASSIGN,
    OP_MODULO_ASSIGN,
} BinaryOperationType;

typedef enum
{
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

typedef enum
{
    FLOW_IF,
    FLOW_MATCH,
} FlowType;

typedef enum
{
    LOOP_LOOP,
    LOOP_WHILE,
    LOOP_FOR,
} LoopType;

typedef enum
{
    STOP_RETURN,
    STOP_BREAK,
    STOP_CONTINUE,
} StopType;

/*** NODE ****/

typedef struct Node Node;

struct Node
{
    NodeType type;
    union
    {
        struct
        {
            Node **statements;
            int count;
        } block;

        struct
        {
            char *name;
        } identifier;

        struct
        {
            char *name;

            bool is_mutable;
            VariableType type;

            Node *value;
        } declaration;

        struct
        {
            char *module_name;

            Node **imported_modules;
            int count;
        } importation;

        struct
        {
            char *name;

            char **arg_names;
            int arg_count;
            VariableType *arg_types;

            VariableType return_type;

            Node *body;
        } function_declaration;

        struct
        {
            char *function_name;

            int arg_count;
            Node **args;
        } function_call;

        struct
        {
            FlowType flow_type;
            Node *condition;

            Node *body;
            Node *else_body; // Only used for if statements
        } flow;

        struct
        {
            LoopType loop_type;
            Node *condition; // Only used for while and for loops

            Node *body;
        } loop;

        struct
        {
            UnaryOperationType op;
            Node *operand;
        } unary_op;

        struct
        {
            BinaryOperationType op;
            Node *left;
            Node *right;
        } binary_op;

        struct
        {
            LogicalOperationType op;
            Node *left;
            Node *right;
        } logical_op;

        struct
        {
            StopType stop_type;
            Node *value; // Only used for return statements
        } stop;

        struct
        {
            VariableType type;
            int int_value;
            float float_value;
            bool bool_value;
            char char_value;
            char *string_value;
        } literal;
    };
};

/*** NODE FUNCTIONS ****/

Node *node_block(Node **statements, int count);
Node *node_identifier(char *name);
Node *node_declaration(char *name, bool is_mutable, VariableType type, Node *value);
Node *node_importation(char *module_name, Node **imported_modules, int count);
Node *node_function_declaration(char *name, char **arg_names, int arg_count, VariableType *arg_types, VariableType return_type, Node *body);
Node *node_function_call(char *function_name, int arg_count, Node **args);
Node *node_flow(FlowType flow_type, Node *condition, Node *body, Node *else_body);
Node *node_loop(LoopType loop_type, Node *condition, Node *body);
Node *node_stop(StopType stop_type, Node *value);
Node *node_unary_op(UnaryOperationType op, Node *operand);
Node *node_binary_op(BinaryOperationType op, Node *left, Node *right);
Node *node_logical_op(LogicalOperationType op, Node *left, Node *right);
Node *node_literal(VariableType type, char *value);

/*** HELPER FUNCTIONS ****/

/**
 * Returns a string representation of the given NodeType.
 * @param type The NodeType to convert to a string.
 * @return A string representation of the NodeType.
 */
char *node_type_to_string(NodeType type);

/**
 * Returns a string representation of the given Node.
 * @param node The Node to convert to a string.
 * @return A string representation of the Node.
 */
char *node_to_string(Node *node);

/**
 * Prints the AST graph starting from the given node.
 * @param node The root node of the AST to print.
 * @param depth The current depth in the AST (used for indentation).
 */
void print_graph(Node *node, int depth);

/**
 * Frees the memory allocated for the given Node and its children.
 * @param node The Node to free.
 */
void free_node(Node *node);

#endif
