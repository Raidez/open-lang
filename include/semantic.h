#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

typedef enum
{
    VARIABLE,
    FUNCTION
} SymbolType;

typedef struct
{
    char *name;
    SymbolType type;
    VariableType variable_type; // Only used for variables
} Symbol;

/**
 * Analyzes the semantics of the given AST rooted at `node`.
 * @param node The root node of the AST to analyze.
 */
void analyze(Node *node);

/**
 * Returns a string representation of the given symbol type.
 * @param type The symbol type to convert to a string.
 * @return A string representation of the symbol type.
 */
char *symbol_type_to_string(SymbolType type);

/**
 * Prints the current symbol table to the console.
 */
void print_symbols();

#endif
