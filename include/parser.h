#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct
{
    Lexer *lexer;
    Token current;
    Token previous;
} Parser;

/**
 * Parses the tokens from the lexer and constructs an abstract syntax tree (AST).
 * @param parser A pointer to the Parser structure containing the lexer and token information.
 * @return A pointer to the root node of the constructed AST.
 */
Node *parse(Parser *parser);

#endif
