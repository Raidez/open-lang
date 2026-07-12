#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include "ast.h"

void codegen(Node *node, FILE *output_file);

#endif
