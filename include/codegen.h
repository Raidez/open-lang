#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>

#include "ast.h"

/**
 * Generates code for the given AST node and writes it to the specified output file.
 * @param node The AST node to generate code for.
 * @param output_file The file to write the generated code to.
 */
void codegen_node(Node *node, FILE *output_file);

/**
 * Generates code for the given AST node and writes it to the specified output file.
 * @param node The AST node to generate code for.
 * @param output_file The file to write the generated code to.
 */
void codegen(Node *node, FILE *output_file);

#endif
