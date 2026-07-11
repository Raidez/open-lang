#include <stdio.h>
#include "utils.h"

void print_tokens(Lexer *lexer)
{
    Token token;
    do
    {
        token = next_token(lexer);
        printf("Token: type=%s, start='%.*s', length=%d, line=%d\n",
               token_type_to_string(token.type), token.length, token.start, token.length, token.line);

    } while (token.type != TOKEN_EOF);
}
