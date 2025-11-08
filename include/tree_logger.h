#ifndef TREE_LOGGER_H_
#define TREE_LOGGER_H_

#include "tree_manager.h"

void PrintNode(TreeNode* node);
void DotPrintNode(TreeNode* node, FILE* file, int rank);

void TreeDotDump(Tree* tree, const char* filename);

#endif // TREE_LOGGER_H_