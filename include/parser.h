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

Node *parse(Parser *parser);

#endif
