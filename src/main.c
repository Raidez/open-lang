#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    bool show_lexer = argv[2] && strcmp(argv[2], "--lexer") == 0;
    bool show_ast = argv[2] && strcmp(argv[2], "--ast") == 0;
    bool show_interpreter = argv[2] && strcmp(argv[2], "--interpreter") == 0;

    // 0. Parse arguments
    char *input_file;
    if (argc < 2)
    {
        fprintf(stderr, "Missing input file.\n");
        return 1;
    }
    input_file = argv[1];

    // 1. Open file

    FILE *file = fopen(input_file, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Can't open file '%s'\n", input_file);
        return 1;
    }

    // 2. Read file

    /// Get the size of the file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    /// Allocate a buffer to hold the file contents
    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    /// Close the file after reading
    fclose(file);

    // 3. Lexer

    Lexer lexer = {.start = buffer, .current = buffer, .line = 1};
    if (show_lexer)
    {
        print_tokens(&lexer); // Uncomment this line to print tokens for debugging
        return 0;             // Exit after printing tokens
    }

    // 4. Parser

    Parser parser = {.lexer = &lexer, .current = next_token(&lexer), .previous = {0}};
    Node *ast = parse(&parser);
    free(buffer); // Free the buffer after parsing

    if (show_ast)
    {
        print_graph(ast, 0); // Uncomment this line to print the AST for debugging
        free_node(ast);      // Free the AST after printing
        return 0;            // Exit after printing the AST
    }

    // 5. Interpret

    if (show_interpreter)
        interpret(ast);

    free_node(ast); // Free the AST after interpretation

    return 0;
}
