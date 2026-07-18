#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantic.h"

static Symbol *symbol_table = NULL;
static int symbol_count = 0;

static Symbol *find_symbol(const char *name)
{
    for (int i = 0; i < 256; i++)
    {
        if (symbol_table[i].name != NULL && strcmp(symbol_table[i].name, name) == 0)
        {
            return &symbol_table[i];
        }
    }
    return NULL;
}

static void analyze_symbols(Node *node)
{
    switch (node->type)
    {
    case NODE_BLOCK:
    {
        for (int i = 0; i < node->block.count; i++)
            analyze_symbols(node->block.statements[i]);
        break;
    }
    case NODE_DECLARATION:
    {
        // Check if the variable is already declared
        if (find_symbol(node->declaration.name) != NULL)
        {
            printf("Error: Variable '%s' is already declared\n", node->declaration.name);
            exit(1);
        }

        // Check if the variable reference is declared before usage
        Node *value_node = node->declaration.value;
        if (value_node != NULL && value_node->type == NODE_IDENTIFIER && find_symbol(value_node->identifier.name) == NULL)
        {
            printf("Error: Variable '%s' is not declared\n", value_node->identifier.name);
            exit(1);
        }

        // Check if the variable is initialized before usage
        if (!node->declaration.is_mutable && value_node == NULL)
        {
            printf("Error: Variable '%s' is not initialized before usage\n", node->declaration.name);
            exit(1);
        }

        if (value_node != NULL && value_node->type == NODE_LITERAL && value_node->literal.type != node->declaration.type)
        {
            printf("Error: Type mismatch, cannot assign %s to %s\n",
                   node_variable_type_to_string(value_node->literal.type),
                   node_variable_type_to_string(node->declaration.type));
            exit(1);
        }

        // Add the variable to the symbol table
        Symbol *symbol = malloc(sizeof(Symbol));
        *symbol = (Symbol){.name = node->declaration.name, .type = VARIABLE, .variable_type = node->declaration.type};
        symbol_table[symbol_count++] = *symbol;
        break;
    }
    default:
        break;
    }
}

void analyze(Node *node)
{
    // 0. Create symbol table

    symbol_table = malloc(sizeof(Symbol) * 256);
    analyze_symbols(node);

    // 1. Check for variable declarations and usages
    // let x: int = 5; // Error: Variable '${name}' is already declared
    // let z: int = x + y; // Error: Variable '${name}' is not declared
    // let y: int; // Error: Variable '${name}' is not initialized before usage

    // 2. Check for variable types and assignments
    // let x: int = "bonjour"; // Error: Type mismatch, cannot assign ${value_type} to ${expected_type}
    // x = "bonjour"; // Error: Type mismatch, cannot assign ${value_type} to ${expected_type}

    // 3. Check for function declarations and calls
    // greet("Raidez"); // Error: Function '${name}' is not declared

    // 4. Check for variable scopes and lifetimes
    // print(x); // Error: Variable '${name}' not exists

    // 5. Check for function return types and values
    // greet(false); // Error: Function '${name}' expects ${expected_type} argument, but received ${value_type}

    // 6. Check for literal operations and types
    // let x: int = true + 5; // Error: Invalid operation between ${value_type} and ${expected_type}

    // 7. Check for keyword usage in specific contexts
    // break; // Error: Keyword '${keyword}' cannot be used outside of a loop or switch statement
}

char *symbol_type_to_string(SymbolType type)
{
    switch (type)
    {
    case VARIABLE:
        return "variable";
    case FUNCTION:
        return "function";
    default:
        return "unknown";
    }
}

void print_symbols()
{
    for (int i = 0; i < symbol_count; i++)
        printf("Symbol: %s, Type: %s, Variable Type: %s\n",
               symbol_table[i].name,
               symbol_type_to_string(symbol_table[i].type),
               node_variable_type_to_string(symbol_table[i].variable_type));
}
