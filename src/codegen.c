#include "codegen.h"
#include "ast.h"

static void handle_declaration(Node *node, FILE *output_file)
{
    // Unmutable variables with const
    if (!node->declaration.is_mutable)
        fprintf(output_file, "const ");

    // C type
    switch (node->declaration.type)
    {
    case TYPE_INFERRED:
        fprintf(output_file, "auto ");
        break;
    case TYPE_INT:
        fprintf(output_file, "int ");
        break;
    case TYPE_FLOAT:
        fprintf(output_file, "float ");
        break;
    case TYPE_BOOL:
        fprintf(output_file, "bool ");
        break;
    case TYPE_CHAR:
        fprintf(output_file, "char ");
        break;
    case TYPE_STRING:
        fprintf(output_file, "char* ");
        break;
    default:
        break;
    }

    // Variable name
    fprintf(output_file, "%s = ", node->declaration.name);

    // Generate code for the value
    codegen_node(node->declaration.value, output_file);

    // Final semicolon
    fprintf(output_file, ";\n");
}

static void handle_literal(Node *node, FILE *output_file)
{
    switch (node->literal.type)
    {
    case TYPE_INT:
        fprintf(output_file, "%d", node->literal.int_value);
        break;
    case TYPE_FLOAT:
        fprintf(output_file, "%f", node->literal.float_value);
        break;
    case TYPE_BOOL:
        fprintf(output_file, "%s", node->literal.bool_value ? "true" : "false");
        break;
    case TYPE_CHAR:
        fprintf(output_file, "'%c'", node->literal.char_value);
        break;
    case TYPE_STRING:
        fprintf(output_file, "\"%s\"", node->literal.string_value);
        break;
    default:
        break;
    }
}

static void handle_binary_op(Node *node, FILE *output_file)
{
    codegen_node(node->binary_op.left, output_file);

    switch (node->binary_op.op)
    {
    case OP_ADD:
        fprintf(output_file, " + ");
        break;
    case OP_SUBTRACT:
        fprintf(output_file, " - ");
        break;
    case OP_MULTIPLY:
        fprintf(output_file, " * ");
        break;
    case OP_DIVIDE:
        fprintf(output_file, " / ");
        break;
    case OP_MODULO:
        fprintf(output_file, " %% ");
        break;
    case OP_ADD_ASSIGN:
        fprintf(output_file, " += ");
        break;
    case OP_SUBTRACT_ASSIGN:
        fprintf(output_file, " -= ");
        break;
    case OP_MULTIPLY_ASSIGN:
        fprintf(output_file, " *= ");
        break;
    case OP_DIVIDE_ASSIGN:
        fprintf(output_file, " /= ");
        break;
    case OP_MODULO_ASSIGN:
        fprintf(output_file, " %%= ");
        break;
    default:
        break;
    }

    codegen_node(node->binary_op.right, output_file);
}

void codegen_node(Node *node, FILE *output_file)
{
    switch (node->type)
    {
    case NODE_BLOCK:
        for (int i = 0; i < node->block.count; i++)
        {
            fprintf(output_file, "    ");
            codegen_node(node->block.statements[i], output_file);
        }
        break;
    case NODE_DECLARATION:
        handle_declaration(node, output_file);
        break;
    case NODE_LITERAL:
        handle_literal(node, output_file);
        break;
    case NODE_BINARY_OP:
        handle_binary_op(node, output_file);
        break;
    default:
        break;
    }
}

void codegen(Node *node, FILE *output_file)
{
    fprintf(output_file, "#include <stdio.h>\n");
    fprintf(output_file, "#include <stdlib.h>\n");
    fprintf(output_file, "#include <stdbool.h>\n");
    fprintf(output_file, "#include <string.h>\n");
    fprintf(output_file, "#include \"std.h\"\n");

    fprintf(output_file, "\nint main() {\n");

    codegen_node(node, output_file);

    fprintf(output_file, "    return 0;\n");
    fprintf(output_file, "}\n");
}
