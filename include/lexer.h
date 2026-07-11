#ifndef LEXER_H
#define LEXER_H

typedef enum
{
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_LET,
    TOKEN_IDENTIFIER,
    TOKEN_EQUAL,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SEMICOLON,
} TokenType;

typedef struct
{
    TokenType type;
    char *start;
    int length;
    int line;
} Token;

typedef struct
{
    char *start;
    char *current;
    int line;
} Lexer;

/**
 * Convert a token type to string
 */
char *token_type_to_string(TokenType type);

/**
 * Get the next token from the lexer
 */
Token next_token(Lexer *lexer);

#endif
