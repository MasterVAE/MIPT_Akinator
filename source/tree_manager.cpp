#include <stdlib.h>
#include <stdio.h>

#include "../include/tree_manager.h"

Tree* TreeConstruct()
{
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    tree->root = NULL;

    return tree;
}

void TreeNodeDestroy(TreeNode* node)
{
    if (node == NULL)   return;

    TreeNodeDestroy(node->left);
    TreeNodeDestroy(node->right);

    free(node);
}
void TreeAdd(Tree* tree, TreeType value)
{
    TreeNode* new_node = (TreeNode*)calloc(1, sizeof(TreeNode));
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;

    if (tree->root == NULL)
    {
        tree->root = new_node;
        return;
    }

    TreeNode* current = tree->root;
    while (true)
    {
        if (value <= current->value)
        {
            if (current->left == NULL)
            {
                current->left = new_node;
                return;
            }
            current = current->left;
        }
        else
        {
            if (current->right == NULL)
            {
                current->right = new_node;
                return;
            }
            current = current->right;
        }
    }
}

void TreeDestroy(Tree* tree)
{
    if (tree->root != NULL)
    {
        TreeNodeDestroy(tree->root);
    }
        
    free(tree);
}