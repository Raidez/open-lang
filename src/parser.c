#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "ast.h"

/*** STATIC ****/

static Token advance(Parser *parser)
{
    parser->previous = parser->current;
    parser->current = next_token(parser->lexer);
    return parser->previous;
}

static Token consume(Parser *parser, TokenType expected)
{
    if (parser->current.type == expected)
    {
        return advance(parser);
    }
    else
    {
        fprintf(stderr, "Expected token type %d but got %d at line %d\n", expected, parser->current.type, parser->current.line);
        exit(1);
    }
}

static Node *parse_let(Parser *parser)
{
    // Consume tokens
    consume(parser, TOKEN_LET);
    Token name = consume(parser, TOKEN_IDENTIFIER);
    consume(parser, TOKEN_EQUAL);
    Token value = consume(parser, TOKEN_NUMBER); // For simplicity, we only handle numbers as values for now
    consume(parser, TOKEN_SEMICOLON);

    // Create a declaration node
    char *declaration_name = strndup(name.start, name.length);
    char *value_str = strndup(value.start, value.length);
    Node *node = node_declaration(declaration_name, false, TYPE_INFERRED, node_int_literal(atoi(value_str)));
    free(value_str);

    return node;
}

/*** PUBLIC ****/

Node *parse(Parser *parser)
{
    if (parser->current.type == TOKEN_LET)
    {
        return parse_let(parser);
    }
    return NULL;
}
