#ifndef TREE_LOGGER_H_
#define TREE_LOGGER_H_

#include "tree_manager.h"

void PrintNode(TreeNode* node);
void DotPrintNode(TreeNode* node, FILE* file, int rank);

void TreeDotDump(Tree* tree);
void TreeDotDumpStart(Tree* tree);
void TreeDotDumpEnd(Tree* tree);

static const char* DOT_FILENAME = "files/tree.dot";

#endif // TREE_LOGGER_H_