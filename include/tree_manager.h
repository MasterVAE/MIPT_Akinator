#ifndef TREE_MANAGER_H_
#define TREE_MANAGER_H_

#include <stdio.h>

typedef int TreeType;

struct TreeNode;

struct TreeNode
{
    TreeType value;

    TreeNode* left;
    TreeNode* right;
};

struct Tree 
{
    FILE* dotfile;

    TreeNode* root;
};

Tree* TreeConstruct();
void TreeNodeDestroy(TreeNode* node);
void TreeAdd(Tree* tree, TreeType value);
void TreeDestroy(Tree* tree);

#endif // TREE_MANAGER_H_