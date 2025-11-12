#ifndef TREE_MANAGER_H_
#define TREE_MANAGER_H_

#include <stdio.h>

typedef char* tree_elem_t;

struct TreeNode
{
    tree_elem_t value;

    TreeNode* parent;

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
TreeNode* NodeConstruct();
void TreeAdd(TreeNode* node, tree_elem_t elem, TreeAddDir direction);
void TreeDestroy(Tree* tree);
void TreeNodeDestroy(TreeNode* node);
AkinatorState SaveTree(Tree* tree, const char* filename);
AkinatorState LoadTree(Tree* tree, const char* filename);
char* LoadToBuffer(const char* filename);

#endif // TREE_MANAGER_H_