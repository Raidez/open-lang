#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/**** STATIC ****/

typedef struct
{
    NodeType type;
    char *name;
    char *(*handler)(Node);
} NodeTypeName;

static char *node_variable_type_to_string(VariableType type)
{
    switch (type)
    {
    case TYPE_INFERRED:
        return "inferred";
    case TYPE_INT:
        return "int";
    case TYPE_FLOAT:
        return "float";
    case TYPE_BOOL:
        return "bool";
    case TYPE_CHAR:
        return "char";
    case TYPE_STRING:
        return "string";
    default:
        return "unknow";
    }
}

static char *node_identifier_to_string(Node node)
{
    char *buffer = malloc(sizeof(char) * 100);
    snprintf(buffer, 100, "name: %s", node.identifier.name);
    return buffer;
}

static char *node_declaration_to_string(Node node)
{
    char *buffer = malloc(sizeof(char) * 100);
    char *is_mutable = node.declaration.is_mutable ? "true" : "false";
    snprintf(buffer, 100, "name: %s, type: %s, mutable: %s",
             node.declaration.name, node_variable_type_to_string(node.declaration.type), is_mutable);
    return buffer;
}

static char *node_literal_to_string(Node node)
{
    char *buffer = malloc(sizeof(char) * 100);
    char *type = node_variable_type_to_string(node.literal.type);

    switch (node.literal.type)
    {
    case TYPE_INT:
        snprintf(buffer, 100, "type: %s, value: %d", type, node.literal.int_value);
        break;
    case TYPE_FLOAT:
        snprintf(buffer, 100, "type: %s, value: %.2f", type, node.literal.float_value);
        break;
    case TYPE_BOOL:
        snprintf(buffer, 100, "type: %s, value: %s", type, node.literal.bool_value ? "true" : "false");
        break;
    case TYPE_CHAR:
        snprintf(buffer, 100, "type: %s, value: '%c'", type, node.literal.char_value);
        break;
    case TYPE_STRING:
        snprintf(buffer, 100, "type: %s, value: \"%s\"", type, node.literal.string_value);
        break;
    default:
        return "";
    }

    return buffer;
}

static char *node_binary_op_to_string(Node node)
{
    char *buffer = malloc(sizeof(char) * 100);
    snprintf(buffer, 100, "op: %d", node.binary_op.op);

    switch (node.binary_op.op)
    {
    case OP_ADD:
        snprintf(buffer, 100, "op: +");
        break;
    case OP_SUBTRACT:
        snprintf(buffer, 100, "op: -");
        break;
    case OP_MULTIPLY:
        snprintf(buffer, 100, "op: *");
        break;
    case OP_DIVIDE:
        snprintf(buffer, 100, "op: /");
        break;
    case OP_MODULO:
        snprintf(buffer, 100, "op: %%");
        break;
    default:
        return "";
    }

    return buffer;
}

static const NodeTypeName node_type_names[] = {
    {NODE_BLOCK, "Block", NULL},
    {NODE_IDENTIFIER, "Identifier", node_identifier_to_string},
    {NODE_DECLARATION, "Declaration", node_declaration_to_string},
    {NODE_IMPORTATION, "Importation", NULL},
    {NODE_FUNCTION_DECLARATION, "Function Declaration", NULL},
    {NODE_FUNCTION_CALL, "Function Call", NULL},
    {NODE_FLOW, "Flow", NULL},
    {NODE_LOOP, "Loop", NULL},
    {NODE_STOP, "Stop", NULL},
    {NODE_UNARY_OP, "Unary Operation", NULL},
    {NODE_BINARY_OP, "Binary Operation", node_binary_op_to_string},
    {NODE_LOGICAL_OP, "Logical Operation", NULL},
    {NODE_LITERAL, "Literal", node_literal_to_string},
};
static int node_type_names_count = sizeof(node_type_names) / sizeof(NodeTypeName);

/**** PUBLIC ****/

Node *node_block(Node **statements, int count)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_BLOCK;
    node->block.statements = statements;
    node->block.count = count;
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

Node *node_importation(char *module_name, Node **imported_modules, int count)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_IMPORTATION;
    node->importation.module_name = module_name;
    node->importation.imported_modules = imported_modules;
    node->importation.count = count;
    return node;
}

Node *node_function_declaration(char *name, char **arg_names, int arg_count, VariableType *arg_types, VariableType return_type, Node *body)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_FUNCTION_DECLARATION;
    node->function_declaration.name = name;
    node->function_declaration.arg_names = arg_names;
    node->function_declaration.arg_count = arg_count;
    node->function_declaration.arg_types = arg_types;
    node->function_declaration.return_type = return_type;
    node->function_declaration.body = body;
    return node;
}

Node *node_function_call(char *function_name, int arg_count, Node **args)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_FUNCTION_CALL;
    node->function_call.function_name = function_name;
    node->function_call.arg_count = arg_count;
    node->function_call.args = args;
    return node;
}

Node *node_flow(FlowType flow_type, Node *condition, Node *body, Node *else_body)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_FLOW;
    node->flow.flow_type = flow_type;
    node->flow.condition = condition;
    node->flow.body = body;
    node->flow.else_body = else_body;
    return node;
}

Node *node_loop(LoopType loop_type, Node *condition, Node *body)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_LOOP;
    node->loop.loop_type = loop_type;
    node->loop.condition = condition;
    node->loop.body = body;
    return node;
}

Node *node_stop(StopType stop_type, Node *value)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_STOP;
    node->stop.stop_type = stop_type;
    node->stop.value = value;
    return node;
}

Node *node_unary_op(UnaryOperationType op, Node *operand)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_UNARY_OP;
    node->unary_op.op = op;
    node->unary_op.operand = operand;
    return node;
}

Node *node_binary_op(BinaryOperationType op, Node *left, Node *right)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_BINARY_OP;
    node->binary_op.op = op;
    node->binary_op.left = left;
    node->binary_op.right = right;
    return node;
}

Node *node_logical_op(LogicalOperationType op, Node *left, Node *right)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_LOGICAL_OP;
    node->logical_op.op = op;
    node->logical_op.left = left;
    node->logical_op.right = right;
    return node;
}

Node *node_literal(VariableType type, char *value)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_LITERAL;
    node->literal.type = type;

    switch (type)
    {
    case TYPE_INT:
        node->literal.int_value = atoi(value);
        break;
    case TYPE_FLOAT:
        node->literal.float_value = atof(value);
        break;
    case TYPE_BOOL:
        node->literal.bool_value = (strcmp(value, "true") == 0);
        break;
    case TYPE_CHAR:
        node->literal.char_value = value[0];
        break;
    case TYPE_STRING:
        node->literal.string_value = strdup(value);
        break;
    default:
        fprintf(stderr, "Unknown literal type\n");
        free(node);
        return NULL;
    }

    return node;
}

char *node_type_to_string(NodeType type)
{
    for (int i = 0; i < node_type_names_count; i++)
    {
        if (node_type_names[i].type == type)
        {
            return node_type_names[i].name;
        }
    }

    fprintf(stderr, "Undefined node type %d", type);
    exit(1);
}

char *node_to_string(Node *node)
{
    char *buffer = malloc(sizeof(char) * 100);
    char *type = NULL;
    char *(*handler)(Node) = NULL;

    for (int i = 0; i < node_type_names_count; i++)
    {
        if (node_type_names[i].type == node->type)
        {
            type = node_type_names[i].name;
            handler = node_type_names[i].handler;
        }
    }

    if (handler)
        snprintf(buffer, 100, "%s { %s }\n", type, handler(*node));
    else
        snprintf(buffer, 100, "%s\n", type);

    return buffer;
}

void print_graph(Node *node, int depth)
{
    if (node == NULL)
        return;

    // Identation for depth
    for (int i = 0; i < depth; i++)
        printf("    ");

    // Display node type
    printf("%s", node_to_string(node));

    // Recursion
    switch (node->type)
    {
    case NODE_BLOCK:
        for (int i = 0; i < node->block.count; i++)
            print_graph(node->block.statements[i], depth + 1);
        break;
    case NODE_DECLARATION:
        print_graph(node->declaration.value, depth + 1);
        break;
    case NODE_BINARY_OP:
        print_graph(node->binary_op.left, depth + 1);
        print_graph(node->binary_op.right, depth + 1);
        break;
    default:
        break;
    }
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
    case NODE_LITERAL:
        switch (node->literal.type)
        {
        case TYPE_STRING:
            free(node->literal.string_value);
            break;
        default:
            break;
        }
        break;
    case NODE_IDENTIFIER:
        free(node->identifier.name);
        break;
    default:
        break;
    }

    free(node);
}
