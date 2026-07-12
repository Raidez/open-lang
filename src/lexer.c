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

typedef struct
{
    char *keyword;
    int length;
    TokenType type;
} Keyword;

typedef struct
{
    TokenType type;
    char *name;
} TokenTypeName;

static const Keyword keywords[] = {
    {"let", 3, TOKEN_LET},
    {"var", 3, TOKEN_VAR},

    {"int", 3, TOKEN_TYPE_INT},
    {"float", 5, TOKEN_TYPE_FLOAT},
    {"bool", 4, TOKEN_TYPE_BOOL},
    {"string", 6, TOKEN_TYPE_STRING},
    {"char", 4, TOKEN_TYPE_CHAR},

    {"true", 4, TOKEN_TRUE_LITERAL},
    {"false", 5, TOKEN_FALSE_LITERAL},

    {"if", 2, TOKEN_IF},
    {"else", 4, TOKEN_ELSE},
    {"match", 5, TOKEN_MATCH},

    {"loop", 4, TOKEN_LOOP},
    {"while", 5, TOKEN_WHILE},
    {"for", 3, TOKEN_FOR},
    {"break", 5, TOKEN_BREAK},
    {"continue", 8, TOKEN_CONTINUE},

    {"func", 4, TOKEN_FUNCTION},
    {"return", 6, TOKEN_RETURN},
};
static const int keywords_count = sizeof(keywords) / sizeof(keywords[0]);

static const Keyword symbols[] = {
    // Multi-character symbols first
    {"->", 2, TOKEN_RETURN_TYPE},
    {"==", 2, TOKEN_EQUAL},
    {"!=", 2, TOKEN_NOT_EQUAL},
    {"<=", 2, TOKEN_LESS_EQUAL},
    {">=", 2, TOKEN_GREATER_EQUAL},
    {"+=", 2, TOKEN_ADD_ASSIGN},
    {"-=", 2, TOKEN_SUBTRACT_ASSIGN},
    {"*=", 2, TOKEN_MULTIPLY_ASSIGN},
    {"/=", 2, TOKEN_DIVIDE_ASSIGN},
    {"%=", 2, TOKEN_MODULO_ASSIGN},
    {"++", 2, TOKEN_INCREMENT},
    {"--", 2, TOKEN_DECREMENT},
    // Single-character symbols after
    {"=", 1, TOKEN_ASSIGN},
    {":", 1, TOKEN_TYPING},
    {",", 1, TOKEN_COMMA},
    {";", 1, TOKEN_SEMICOLON},
    {"(", 1, TOKEN_LPAREN},
    {")", 1, TOKEN_RPAREN},
    {"{", 1, TOKEN_LBRACE},
    {"}", 1, TOKEN_RBRACE},
    {"<", 1, TOKEN_LESS},
    {">", 1, TOKEN_GREATER},
    {"+", 1, TOKEN_ADD},
    {"-", 1, TOKEN_SUBTRACT},
    {"*", 1, TOKEN_MULTIPLY},
    {"/", 1, TOKEN_DIVIDE},
    {"%", 1, TOKEN_MODULO},
};
static const int symbols_count = sizeof(symbols) / sizeof(symbols[0]);

static const TokenTypeName token_type_names[] = {
    {TOKEN_EOF, "EOF"},
    {TOKEN_ERROR, "ERROR"},
    {TOKEN_EMPTY, "EMPTY"},
    {TOKEN_LET, "LET"},
    {TOKEN_VAR, "VAR"},
    {TOKEN_IDENTIFIER, "IDENTIFIER"},
    {TOKEN_ASSIGN, "ASSIGN"},
    {TOKEN_TYPING, "TYPING"},
    {TOKEN_TYPE_INT, "TYPE_INT"},
    {TOKEN_TYPE_FLOAT, "TYPE_FLOAT"},
    {TOKEN_TYPE_BOOL, "TYPE_BOOL"},
    {TOKEN_TYPE_STRING, "TYPE_STRING"},
    {TOKEN_TYPE_CHAR, "TYPE_CHAR"},
    {TOKEN_INCREMENT, "INCREMENT"},
    {TOKEN_DECREMENT, "DECREMENT"},
    {TOKEN_ADD_ASSIGN, "ADD_ASSIGN"},
    {TOKEN_SUBTRACT_ASSIGN, "SUBTRACT_ASSIGN"},
    {TOKEN_MULTIPLY_ASSIGN, "MULTIPLY_ASSIGN"},
    {TOKEN_DIVIDE_ASSIGN, "DIVIDE_ASSIGN"},
    {TOKEN_MODULO_ASSIGN, "MODULO_ASSIGN"},
    {TOKEN_EQUAL, "EQUAL"},
    {TOKEN_NOT_EQUAL, "NOT_EQUAL"},
    {TOKEN_LESS, "LESS"},
    {TOKEN_GREATER, "GREATER"},
    {TOKEN_LESS_EQUAL, "LESS_EQUAL"},
    {TOKEN_GREATER_EQUAL, "GREATER_EQUAL"},
    {TOKEN_ADD, "ADD"},
    {TOKEN_SUBTRACT, "SUBTRACT"},
    {TOKEN_MULTIPLY, "MULTIPLY"},
    {TOKEN_DIVIDE, "DIVIDE"},
    {TOKEN_MODULO, "MODULO"},
    {TOKEN_NUMBER_LITERAL, "NUMBER_LITERAL"},
    {TOKEN_FLOAT_LITERAL, "FLOAT_LITERAL"},
    {TOKEN_BOOLEAN_LITERAL, "BOOLEAN_LITERAL"},
    {TOKEN_STRING_LITERAL, "STRING_LITERAL"},
    {TOKEN_CHAR_LITERAL, "CHAR_LITERAL"},
    {TOKEN_TRUE_LITERAL, "TRUE_LITERAL"},
    {TOKEN_FALSE_LITERAL, "FALSE_LITERAL"},
    {TOKEN_IF, "IF"},
    {TOKEN_ELSE, "ELSE"},
    {TOKEN_MATCH, "MATCH"},
    {TOKEN_LOOP, "LOOP"},
    {TOKEN_WHILE, "WHILE"},
    {TOKEN_FOR, "FOR"},
    {TOKEN_BREAK, "BREAK"},
    {TOKEN_CONTINUE, "CONTINUE"},
    {TOKEN_FUNCTION, "FUNCTION"},
    {TOKEN_RETURN_TYPE, "RETURN_TYPE"},
    {TOKEN_RETURN, "RETURN"},
    {TOKEN_LPAREN, "LPAREN"},
    {TOKEN_RPAREN, "RPAREN"},
    {TOKEN_LBRACE, "LBRACE"},
    {TOKEN_RBRACE, "RBRACE"},
    {TOKEN_COMMA, "COMMA"},
    {TOKEN_SEMICOLON, "SEMICOLON"},
};
static const int token_type_names_count = sizeof(token_type_names) / sizeof(token_type_names[0]);

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
        return (Token){TOKEN_STRING_LITERAL, lexer->start + 1, (int)(lexer->current - lexer->start - 2), lexer->line};
    }
    else
    {
        return (Token){TOKEN_CHAR_LITERAL, lexer->start + 1, (int)(lexer->current - lexer->start - 2), lexer->line};
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
    for (int i = 0; i < keywords_count; i++)
    {
        if (length == keywords[i].length && strncmp(lexer->start, keywords[i].keyword, length) == 0)
            return make_token(lexer, keywords[i].type);
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
        return make_token(lexer, TOKEN_FLOAT_LITERAL);

    return make_token(lexer, TOKEN_NUMBER_LITERAL);
}

/**
 * Handle symbol tokens (single and multi-character symbols)
 */
static Token handle_symbol_token(Lexer *lexer, char c)
{
    for (int i = 0; i < symbols_count; i++)
    {
        if (c == symbols[i].keyword[0])
        {
            // Check for multi-character symbols
            if (symbols[i].length == 2 && peek(lexer) == symbols[i].keyword[1])
            {
                advance(lexer);
                return make_token(lexer, symbols[i].type);
            }
            else if (symbols[i].length == 1)
                return make_token(lexer, symbols[i].type);
        }
    }

    return empty_token(lexer);
}

/**** PUBLIC ****/

char *token_type_to_string(TokenType type)
{
    for (int i = 0; i < token_type_names_count; i++)
    {
        if (token_type_names[i].type == type)
            return token_type_names[i].name;
    }
    return "UNKNOWN";
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
    Token token = handle_symbol_token(lexer, c);
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
