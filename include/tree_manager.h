#ifndef TREE_MANAGER_H_
#define TREE_MANAGER_H_

#include <stdio.h>

struct TreeNode;

struct TreeNode
{
    char* string;

    TreeNode* left;
    TreeNode* right;
};

struct Tree 
{
    TreeNode* root;
};

enum TreeAddDir
{
    TREE_ADD_LEFT,
    TREE_ADD_RIGHT
};

enum AkinatorState
{
    AKINATOR_CORRECT,
    AKINATOR_FILE_ERROR,
    AKINATOR_BUFFER_ERROR
};

Tree* TreeConstruct();
void TreeAdd(TreeNode* node, char* string, TreeAddDir direction);
void TreeDestroy(Tree* tree);
AkinatorState SaveTree(Tree* tree, const char* filename);
AkinatorState LoadTree(Tree* tree, const char* filename);
char* LoadToBuffer(const char* filename);

#endif // TREE_MANAGER_H_