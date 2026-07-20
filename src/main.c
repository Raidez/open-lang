#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "interpreter.h"
#include "codegen.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    // 1. Parse command-line arguments

    printf("1. Parse command-line arguments\n");
    argparse(argc, argv);
    bool show_lexer = is_flag_set("--lexer");
    bool show_ast = is_flag_set("--ast");
    bool show_symbols = is_flag_set("--symbols");
    bool show_interpreter = is_flag_set("--interpreter");
    char *input_file = get_argument_value("input_file");

    // 2. Read file
    printf("\n2. Read file\n");
    char *buffer = read_file(input_file);
    if (buffer == NULL)
    {
        return 1;
    }

    // 3. Lexer

    printf("\n3. Tokenization\n");
    Lexer lexer = {.start = buffer, .current = buffer, .line = 1};
    if (show_lexer)
    {
        printf("\n========== Tokens ==========\n");
        print_tokens(&lexer);
    }

    // 4. Parser

    printf("\n4. Parsing\n");
    Parser parser = {.lexer = &lexer, .current = next_token(&lexer), .previous = {0}};
    Node *ast = parse(&parser);

    if (show_ast)
    {
        printf("\n========== AST ==========\n");
        print_graph(ast, 0);
    }

    // 5. Semantic Analysis

    printf("\n5. Semantic Analysis\n");
    analyze(ast);

    if (show_symbols)
    {
        printf("\n========== Symbols ==========\n");
        print_symbols();
    }

    // 6. Interpret / Compile

    if (show_interpreter)
    {
        printf("\n6. Interpreter\n");
        interpret(ast);
    }
    else
    {
        printf("\n6. Compilation\n");
        FILE *output_file = fopen("output/src/output.c", "w");
        codegen(ast, output_file);
        fclose(output_file);
    }

    // 7. Cleanup

    printf("\n7. Cleanup\n");
    free(buffer);   // Free the buffer
    free_node(ast); // Free the AST

    return 0;
}
