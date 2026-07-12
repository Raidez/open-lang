#ifndef LEXER_H
#define LEXER_H

typedef enum
{
    TOKEN_EOF,   // End of file
    TOKEN_ERROR, // Error token
    TOKEN_EMPTY, // Empty token (used internally)

    TOKEN_LET,
    TOKEN_VAR,
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN,
    TOKEN_TYPING,

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

    TOKEN_NUMBER_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_BOOLEAN_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_TRUE_LITERAL,
    TOKEN_FALSE_LITERAL,

    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_MATCH,

    TOKEN_LOOP,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_BREAK,
    TOKEN_CONTINUE,

    TOKEN_FUNCTION,
    TOKEN_RETURN_TYPE,
    TOKEN_RETURN,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,

    TOKEN_COMMA,
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
