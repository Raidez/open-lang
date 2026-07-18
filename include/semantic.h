#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

/**
 * Analyzes the semantics of the given AST rooted at `node`.
 * @param node The root node of the AST to analyze.
 */
void analyze_semantics(Node *node);

#endif
