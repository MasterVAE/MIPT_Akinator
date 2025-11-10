#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/tree_manager.h"
#include "../include/akinator_manager.h"

static void SaveNode(TreeNode* node, FILE* file);
static TreeNode* NodeConstruct();
static void TreeNodeDestroy(TreeNode* node);
static char* LoadNode(TreeNode* node, char* buffer);
static size_t FileLength(FILE* file);
static char* CreateNewBase();

Tree* TreeConstruct()
{
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    if(!tree) return NULL;

    tree->root = NodeConstruct();
    return tree;
}

static TreeNode* NodeConstruct()
{
    TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
    if(!node) return NULL;

    node->string = NULL;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static void TreeNodeDestroy(TreeNode* node)
{
    if(!node) return;

    TreeNodeDestroy(node->left);
    TreeNodeDestroy(node->right);

    free(node->string);
    free(node);
}

void TreeAdd(TreeNode* node, char* string, TreeAddDir direction)
{
    if(!node) return;
    if(!string) return;

    TreeNode* new_node = NodeConstruct();
    new_node->string = string;

    if(direction == TREE_ADD_LEFT)
    {
        if(node->left)
        {
            TreeNodeDestroy(node->left);
        }
        node->left = new_node;
        return;
    }

    if(node->right)
    {
        TreeNodeDestroy(node->right);   
    }
    node->right = new_node;
}

void TreeDestroy(Tree* tree)
{
    if(!tree) return;

    if(tree->root)
    {
        TreeNodeDestroy(tree->root);
    }
        
    free(tree);
}

AkinatorState SaveTree(Tree* tree, const char* filename)
{   
    if(!tree) return AKINATOR_BUFFER_ERROR;
    if(!filename) return AKINATOR_FILE_ERROR;

    FILE* file = fopen(filename, "w+");
    if(!file) return AKINATOR_FILE_ERROR;

    SaveNode(tree->root, file);
    fclose(file);

    return AKINATOR_CORRECT;
}

static void SaveNode(TreeNode* node, FILE* file)
{
    if(!node || !file) return;

    fprintf(file, "(\"%s\"", node->string);
    
    SaveNode(node->left, file);
    SaveNode(node->right, file);

    fprintf(file, ")");
}

AkinatorState LoadTree(Tree* tree, const char* filename)
{
    if(!tree) return AKINATOR_BUFFER_ERROR;
    if(!filename) return AKINATOR_FILE_ERROR;

    char* buffer = LoadToBuffer(filename);
    if(!buffer) return AKINATOR_BUFFER_ERROR;

    LoadNode(tree->root, buffer);
    free(buffer);

    return AKINATOR_CORRECT;
}

static char* LoadNode(TreeNode* node, char* buffer)
{
    if(!node || !buffer) return NULL;

    buffer += 2;
    size_t len = 0;
    char c;
    while((c = buffer[len++]) != '\"' && c != '\0');

    node->string = (char*)calloc(len, sizeof(char));
    
    strncpy(node->string, buffer, len-1);

    buffer += len;

    if(buffer[0] == ')') return buffer;

    TreeNode* left_node = NodeConstruct();
    node->left = left_node;
    buffer = LoadNode(left_node, buffer) + 1;

    TreeNode* right_node = NodeConstruct();
    node->right = right_node;
    buffer = LoadNode(right_node, buffer);
    return buffer + 1;
}

char* LoadToBuffer(const char* filename)
{
    if(!filename) return CreateNewBase();

    FILE* file = fopen(filename, "r");
    if(!file) return CreateNewBase();

    size_t filesize = FileLength(file);

    char* buffer = (char*)calloc(filesize + 1, sizeof(char));
    if(!buffer)
    {
        fclose(file);
        return NULL;
    }

    fread(buffer, sizeof(char), filesize, file);
    buffer[filesize] = '\0';

    fclose(file);
    return buffer;
}

static size_t FileLength(FILE* file)
{
    if(!file) return 0;

    fseek(file, 0, SEEK_END);
    size_t filesize = (size_t)ftell(file);
    fseek(file, 0, SEEK_SET);

    return filesize;
}

static char* CreateNewBase()
{
    char* buffer = (char*)calloc(strlen(DEFAULT_LOAD_VALUE) + 1, sizeof(char));
    strcpy(buffer, DEFAULT_LOAD_VALUE);
    return buffer;
}