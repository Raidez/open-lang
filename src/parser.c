#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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
        fprintf(stderr, "Expected token type %s but got %s at line %d\n",
                token_type_to_string(expected), token_type_to_string(parser->current.type), parser->current.line);
        exit(1);
    }
}

static int get_binding_power(TokenType type)
{
    switch (type)
    {
    case TOKEN_ADD:
        return 10;
    case TOKEN_SUBTRACT:
        return 10;
    case TOKEN_MULTIPLY:
        return 20;
    case TOKEN_DIVIDE:
        return 20;
    case TOKEN_MODULO:
        return 20;
    default:
        return 0;
    }
}

static Node *parse_literal(Parser *parser)
{
    Node *literal;
    Token token = advance(parser);
    char *s = strndup(token.start, token.length);

    switch (token.type)
    {
    case TOKEN_INT_LITERAL:
        literal = node_literal(TYPE_INT, s);
        break;
    case TOKEN_FLOAT_LITERAL:
        literal = node_literal(TYPE_FLOAT, s);
        break;
    case TOKEN_CHAR_LITERAL:
        literal = node_literal(TYPE_CHAR, s);
        break;
    case TOKEN_STRING_LITERAL:
        literal = node_literal(TYPE_STRING, s);
        break;
    case TOKEN_TRUE_LITERAL:
    case TOKEN_FALSE_LITERAL:
        literal = node_literal(TYPE_BOOL, s);
        break;
    default:
        fprintf(stderr, "Unexpected token %s in literal\n", token_type_to_string(token.type));
        exit(1);
    }

    free(s);
    return literal;
}

static Node *parse_expression(Parser *parser, int min_bp)
{
    // Left part
    Node *left;

    switch (parser->current.type)
    {
    case TOKEN_INT_LITERAL:
    case TOKEN_FLOAT_LITERAL:
    case TOKEN_CHAR_LITERAL:
    case TOKEN_STRING_LITERAL:
    case TOKEN_TRUE_LITERAL:
    case TOKEN_FALSE_LITERAL:
        left = parse_literal(parser);
        break;
    case TOKEN_IDENTIFIER:
    {
        Token token = advance(parser);
        char *name = strndup(token.start, token.length);
        left = node_identifier(name);
        break;
    }
    case TOKEN_LPAREN:
    {
        advance(parser);
        left = parse_expression(parser, 0);
        consume(parser, TOKEN_RPAREN);
        break;
    }
    default:
        fprintf(stderr, "Unexpected token %s in expression\n", token_type_to_string(parser->current.type));
        exit(1);
    }

    // Right part
    while (true)
    {
        int bp = get_binding_power(parser->current.type);
        if (bp <= min_bp)
            break;

        Token op = advance(parser);
        Node *right = parse_expression(parser, bp);

        BinaryOperationType op_type;
        switch (op.type)
        {
        case TOKEN_ADD:
            op_type = OP_ADD;
            break;
        case TOKEN_SUBTRACT:
            op_type = OP_SUBTRACT;
            break;
        case TOKEN_MULTIPLY:
            op_type = OP_MULTIPLY;
            break;
        case TOKEN_DIVIDE:
            op_type = OP_DIVIDE;
            break;
        case TOKEN_MODULO:
            op_type = OP_MODULO;
            break;
        default:
            fprintf(stderr, "Unexpected token %s in expression\n", token_type_to_string(parser->current.type));
            exit(1);
        }

        left = node_binary_op(op_type, left, right);
    }

    return left;
}

static Node *parse_declaration(Parser *parser)
{
    // Consume tokens
    Token token_declaration = advance(parser);
    Token token_name = consume(parser, TOKEN_IDENTIFIER);
    Token token_next = advance(parser);

    Token token_type = {.type = TOKEN_EMPTY, .start = NULL, .length = 0, .line = 0};
    if (token_next.type == TOKEN_TYPING)
    {
        token_type = advance(parser);
        consume(parser, TOKEN_ASSIGN);
    }

    Node *node_value = parse_expression(parser, 0);
    consume(parser, TOKEN_SEMICOLON);

    // Create a declaration node
    char *name = strndup(token_name.start, token_name.length);
    bool is_mutable = (token_declaration.type == TOKEN_VAR);

    VariableType type = TYPE_INFERRED;
    if (token_type.type != TOKEN_EMPTY)
    {
        switch (token_type.type)
        {
        case TOKEN_TYPE_INT:
            type = TYPE_INT;
            break;
        case TOKEN_TYPE_FLOAT:
            type = TYPE_FLOAT;
            break;
        case TOKEN_TYPE_CHAR:
            type = TYPE_CHAR;
            break;
        case TOKEN_TYPE_STRING:
            type = TYPE_STRING;
            break;
        case TOKEN_TYPE_BOOL:
            type = TYPE_BOOL;
            break;
        default:
            fprintf(stderr, "Unknown type token %s at line %d\n",
                    token_type_to_string(token_type.type), token_type.line);
            exit(1);
        }
    }

    Node *node = node_declaration(name, is_mutable, type, node_value);

    return node;
}

/*** PUBLIC ****/

Node *parse(Parser *parser)
{
    // Wrap inside a node block
    Node **statements = malloc(sizeof(Node *) * 256);
    int count = 0;

    while (parser->current.type != TOKEN_EOF)
    {
        if (parser->current.type == TOKEN_LET || parser->current.type == TOKEN_VAR)
        {
            statements[count++] = parse_declaration(parser);
        }
        else
        {
            fprintf(stderr, "Unexpected token %*s at line %d\n", parser->current.length, parser->current.start, parser->current.line);
            exit(1);
        }
    }

    return node_block(statements, count);
}
