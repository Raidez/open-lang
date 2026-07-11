#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

/**** STATIC ****/

typedef enum
{
    ALPHA,
    NUMERIC,
    ALPHANUMERIC,
    STRING,
} WordType;

/**
 * Peek the current character without advance
 */
static char peek(Lexer *lexer)
{
    return *lexer->current;
}

/**
 * Advance the current character and return it
 */
static char advance(Lexer *lexer)
{
    return *lexer->current++;
}

/**
 * Advance the current character while it matches the given word type
 */
static void advance_word(Lexer *lexer, WordType word_type)
{
    if (word_type == ALPHA)
    {
        while (isalpha(peek(lexer)) || peek(lexer) == '_')
        {
            advance(lexer);
        }
    }
    else if (word_type == NUMERIC)
    {

        while (isdigit(peek(lexer)))
        {
            advance(lexer);
        }
    }
    else if (word_type == ALPHANUMERIC)
    {

        while (isalnum(peek(lexer)) || peek(lexer) == '_')
        {
            advance(lexer);
        }
    }
    else if (word_type == STRING)
    {
        while (peek(lexer) != '"' && peek(lexer) != '\0')
        {
            if (peek(lexer) == '\n')
            {
                lexer->line++;
            }
            advance(lexer);
        }
    }
}

/**
 * Skip whitespace and comments
 */
static void skip_whitespace(Lexer *lexer)
{
    while (1)
    {
        switch (peek(lexer))
        {
        case ' ':
        case '\r':
        case '\t':
            advance(lexer);
            break;
        case '\n':
            lexer->line++;
            advance(lexer);
            break;
        default:
            return;
        }
    }
}

/**
 * Make a token from the current lexer state
 */
static Token make_token(Lexer *lexer, TokenType type)
{
    return (Token){type, lexer->start, (int)(lexer->current - lexer->start), lexer->line};
}

/**** PUBLIC ****/

char *token_type_to_string(TokenType type)
{
    switch (type)
    {
    case TOKEN_EOF:
        return "EOF";
    case TOKEN_ERROR:
        return "ERROR";
    case TOKEN_LET:
        return "LET";
    case TOKEN_IDENTIFIER:
        return "IDENTIFIER";
    case TOKEN_EQUAL:
        return "EQUAL";
    case TOKEN_NUMBER:
        return "NUMBER";
    case TOKEN_STRING:
        return "STRING";
    case TOKEN_LPAREN:
        return "LPAREN";
    case TOKEN_RPAREN:
        return "RPAREN";
    case TOKEN_SEMICOLON:
        return "SEMICOLON";
    default:
        return "UNKNOWN";
    }
}

Token next_token(Lexer *lexer)
{
    // Skip whitespace and comments
    skip_whitespace(lexer);

    // Mark the start of the next token
    lexer->start = lexer->current;

    // Check for end of file
    if (peek(lexer) == '\0')
    {
        return make_token(lexer, TOKEN_EOF);
    }

    char c = advance(lexer);

    // Check for single-character tokens
    switch (c)
    {
    case '=':
        return make_token(lexer, TOKEN_EQUAL);
    case ';':
        return make_token(lexer, TOKEN_SEMICOLON);
    case '(':
        return make_token(lexer, TOKEN_LPAREN);
    case ')':
        return make_token(lexer, TOKEN_RPAREN);
    }

    // Check for string literals
    if (c == '"')
    {
        advance_word(lexer, STRING);

        // Check if the string literal is properly terminated
        if (peek(lexer) == '"')
        {
            advance(lexer); // Consume the closing quote

            // Return the string token, excluding the quotes
            return (Token){TOKEN_STRING, lexer->start + 1, (int)(lexer->current - lexer->start - 2), lexer->line};
        }
        else
        {
            fprintf(stderr, "Unterminated string literal at line %d\n", lexer->line);
            return make_token(lexer, TOKEN_ERROR);
        }
    }

    // Check for number tokens
    if (isdigit(c))
    {
        advance_word(lexer, NUMERIC);
        return make_token(lexer, TOKEN_NUMBER);
    }

    // Check for word tokens (identifiers and keywords)
    if (isalpha(c) || c == '_')
    {
        advance_word(lexer, ALPHANUMERIC);

        // Check if the identifier is a keyword
        int length = (int)(lexer->current - lexer->start);
        if (length == 3 && strncmp(lexer->start, "let", 3) == 0)
        {
            return make_token(lexer, TOKEN_LET);
        }
        else
        {
            return make_token(lexer, TOKEN_IDENTIFIER);
        }
    }

    fprintf(stderr, "Unexpected character '%c' at line %d\n", c, lexer->line);
    return make_token(lexer, TOKEN_ERROR); // Default to error token
}
