#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
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

        while (isdigit(peek(lexer)) || peek(lexer) == '.')
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

/**
 * Create an error token with the given message
 */
static Token error_token(Lexer *lexer, const char *message)
{
    fprintf(stderr, "Lexer error at line %d: %s\n", lexer->line, message);
    return (Token){TOKEN_ERROR, lexer->start, (int)(lexer->current - lexer->start), lexer->line};
}

/**
 * Create an empty token
 */
static Token empty_token(Lexer *lexer)
{
    return (Token){TOKEN_EMPTY, lexer->start, 0, lexer->line};
}

/**
 * Create a textual token (string or char literal) from the current lexer state
 */
static Token textual_token(Lexer *lexer, bool is_string)
{
    if (is_string)
    {
        return (Token){TOKEN_STRING, lexer->start + 1, (int)(lexer->current - lexer->start - 2), lexer->line};
    }
    else
    {
        return (Token){TOKEN_CHAR, lexer->start + 1, (int)(lexer->current - lexer->start - 2), lexer->line};
    }
}

/**
 * Handle word tokens (identifiers and keywords)
 */
static Token handle_word_token(Lexer *lexer)
{
    advance_word(lexer, ALPHANUMERIC);

    // Check if the identifier is a keyword
    int length = (int)(lexer->current - lexer->start);
    if (length == 3 && strncmp(lexer->start, "let", 3) == 0)
    {
        return make_token(lexer, TOKEN_LET);
    }
    else if (length == 3 && strncmp(lexer->start, "var", 3) == 0)
    {
        return make_token(lexer, TOKEN_VAR);
    }
    else if (length == 4 && strncmp(lexer->start, "true", 4) == 0)
    {
        return make_token(lexer, TOKEN_TRUE);
    }
    else if (length == 5 && strncmp(lexer->start, "false", 5) == 0)
    {
        return make_token(lexer, TOKEN_FALSE);
    }
    return make_token(lexer, TOKEN_IDENTIFIER);
}

/**
 * Handle textual tokens (string & char literals)
 */
static Token handle_textual_token(Lexer *lexer, char c)
{
    if (c == '\'')
    {
        advance(lexer);

        // Check if the char literal is properly terminated and is a single character
        if (peek(lexer) == '\'' && (lexer->current - lexer->start == 2))
        {
            advance(lexer); // Consume the closing quote
            return textual_token(lexer, false);
        }
        else
            return error_token(lexer, "Unterminated char literal");
    }
    else if (c == '"')
    {
        advance_word(lexer, STRING);

        // Check if the string literal is properly terminated
        if (peek(lexer) == '"')
        {
            advance(lexer); // Consume the closing quote
            return textual_token(lexer, true);
        }
        else
            return error_token(lexer, "Unterminated string literal");
    }
    else
        return error_token(lexer, "Unexpected character");
}

/**
 * Handle numeric tokens (integer & float literals)
 */
static Token handle_numeric_token(Lexer *lexer)
{
    advance_word(lexer, NUMERIC);

    // Check if the number is float
    int length = (int)(lexer->current - lexer->start);
    if (memchr(lexer->start, '.', length) != NULL)
        return make_token(lexer, TOKEN_FLOAT);

    return make_token(lexer, TOKEN_NUMBER);
}

/**
 * Handle operator tokens (single and multi-character operators)
 */
static Token handle_operator_token(Lexer *lexer, char c)
{

    switch (c)
    {
    case ';':
        return make_token(lexer, TOKEN_SEMICOLON);
    case '(':
        return make_token(lexer, TOKEN_LPAREN);
    case ')':
        return make_token(lexer, TOKEN_RPAREN);
    case '{':
        return make_token(lexer, TOKEN_LBRACE);
    case '}':
        return make_token(lexer, TOKEN_RBRACE);
    case '=':
    {
        // Check for == operator
        if (peek(lexer) == '=')
        {
            advance(lexer);
            return make_token(lexer, TOKEN_EQUAL);
        }
        return make_token(lexer, TOKEN_ASSIGN);
    }
    case '+':
    {
        // Check for += operator
        if (peek(lexer) == '=')
        {
            advance(lexer);
            return make_token(lexer, TOKEN_ADD_ASSIGN);
        }
        // Check for ++ operator
        else if (peek(lexer) == '+')
        {
            advance(lexer);
            return make_token(lexer, TOKEN_INCREMENT);
        }
        return make_token(lexer, TOKEN_ADD);
    }
    case '-':
    {
        // Check for -= operator
        if (peek(lexer) == '=')
        {
            advance(lexer);
            return make_token(lexer, TOKEN_SUBTRACT_ASSIGN);
        }
        // Check for -- operator
        else if (peek(lexer) == '-')
        {
            advance(lexer);
            return make_token(lexer, TOKEN_DECREMENT);
        }
        return make_token(lexer, TOKEN_SUBTRACT);
    }
    case '*':
    {
        if (peek(lexer) == '=')
        {
            advance(lexer);
            return make_token(lexer, TOKEN_MULTIPLY_ASSIGN);
        }
        return make_token(lexer, TOKEN_MULTIPLY);
    }
    case '/':
    {
        if (peek(lexer) == '=')
        {
            advance(lexer);
            return make_token(lexer, TOKEN_DIVIDE_ASSIGN);
        }
        return make_token(lexer, TOKEN_DIVIDE);
    }
    case '%':
    {
        if (peek(lexer) == '=')
        {
            advance(lexer);
            return make_token(lexer, TOKEN_MODULO_ASSIGN);
        }
        return make_token(lexer, TOKEN_MODULO);
    }
    case '<':
    {
        if (peek(lexer) == '=')
        {
            advance(lexer);
            return make_token(lexer, TOKEN_LESS_EQUAL);
        }
        return make_token(lexer, TOKEN_LESS);
    }
    case '>':
    {
        if (peek(lexer) == '=')
        {
            advance(lexer);
            return make_token(lexer, TOKEN_GREATER_EQUAL);
        }
        return make_token(lexer, TOKEN_GREATER);
    }
    }

    return empty_token(lexer);
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
    case TOKEN_EMPTY:
        return "EMPTY";
    case TOKEN_LET:
        return "LET";
    case TOKEN_VAR:
        return "VAR";
    case TOKEN_IDENTIFIER:
        return "IDENTIFIER";
    case TOKEN_ASSIGN:
        return "ASSIGN";
    case TOKEN_TYPE_INT:
        return "TYPE_INT";
    case TOKEN_TYPE_FLOAT:
        return "TYPE_FLOAT";
    case TOKEN_TYPE_BOOL:
        return "TYPE_BOOL";
    case TOKEN_TYPE_STRING:
        return "TYPE_STRING";
    case TOKEN_TYPE_CHAR:
        return "TYPE_CHAR";
    case TOKEN_INCREMENT:
        return "INCREMENT";
    case TOKEN_DECREMENT:
        return "DECREMENT";
    case TOKEN_ADD_ASSIGN:
        return "ADD_ASSIGN";
    case TOKEN_SUBTRACT_ASSIGN:
        return "SUBTRACT_ASSIGN";
    case TOKEN_MULTIPLY_ASSIGN:
        return "MULTIPLY_ASSIGN";
    case TOKEN_DIVIDE_ASSIGN:
        return "DIVIDE_ASSIGN";
    case TOKEN_MODULO_ASSIGN:
        return "MODULO_ASSIGN";
    case TOKEN_EQUAL:
        return "EQUAL";
    case TOKEN_NOT_EQUAL:
        return "NOT_EQUAL";
    case TOKEN_LESS:
        return "LESS";
    case TOKEN_GREATER:
        return "GREATER";
    case TOKEN_LESS_EQUAL:
        return "LESS_EQUAL";
    case TOKEN_GREATER_EQUAL:
        return "GREATER_EQUAL";
    case TOKEN_ADD:
        return "ADD";
    case TOKEN_SUBTRACT:
        return "SUBTRACT";
    case TOKEN_MULTIPLY:
        return "MULTIPLY";
    case TOKEN_DIVIDE:
        return "DIVIDE";
    case TOKEN_MODULO:
        return "MODULO";
    case TOKEN_NUMBER:
        return "NUMBER";
    case TOKEN_FLOAT:
        return "FLOAT";
    case TOKEN_BOOLEAN:
        return "BOOLEAN";
    case TOKEN_STRING:
        return "STRING";
    case TOKEN_CHAR:
        return "CHAR";
    case TOKEN_TRUE:
        return "TRUE";
    case TOKEN_FALSE:
        return "FALSE";
    case TOKEN_LPAREN:
        return "LPAREN";
    case TOKEN_RPAREN:
        return "RPAREN";
    case TOKEN_LBRACE:
        return "LBRACE";
    case TOKEN_RBRACE:
        return "RBRACE";
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
        return make_token(lexer, TOKEN_EOF);

    // Tokenize next character

    char c = advance(lexer);

    // Check for operator tokens
    Token token = handle_operator_token(lexer, c);
    if (token.type != TOKEN_EMPTY)
        return token;

    // Check for string literals
    if (c == '\'' || c == '"')
        return handle_textual_token(lexer, c);

    // Check for number tokens
    if (isdigit(c) || (c == '.' && isdigit(peek(lexer))))
        return handle_numeric_token(lexer);

    // Check for word tokens (identifiers and keywords)
    if (isalpha(c) || c == '_')
        return handle_word_token(lexer);

    return error_token(lexer, "Unexpected character");
}
