#ifndef LEXER_H
#define LEXER_H

typedef enum
{
    TOKEN_EOF,
    TOKEN_ERROR,

    TOKEN_LET,
    TOKEN_VAR,
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN,

    TOKEN_TYPE_INT,
    TOKEN_TYPE_FLOAT,
    TOKEN_TYPE_BOOL,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_CHAR,

    TOKEN_INCREMENT,
    TOKEN_DECREMENT,

    TOKEN_ADD_ASSIGN,
    TOKEN_SUBTRACT_ASSIGN,
    TOKEN_MULTIPLY_ASSIGN,
    TOKEN_DIVIDE_ASSIGN,
    TOKEN_MODULO_ASSIGN,

    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS,
    TOKEN_GREATER,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,

    TOKEN_ADD,
    TOKEN_SUBTRACT,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MODULO,

    TOKEN_NUMBER,
    TOKEN_FLOAT,
    TOKEN_BOOLEAN,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_TRUE,
    TOKEN_FALSE,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,

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
