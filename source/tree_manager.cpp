#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "tree_manager.h"
#include "akinator_manager.h"

static void SaveNode(TreeNode* node, FILE* file, size_t rank);
static char* LoadNode(TreeNode* node, char* buffer);
static size_t FileLength(FILE* file);
static char* SkipSpaces(char* string);
static char* CreateNewBase();

Tree* TreeConstruct()
{
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    assert(tree);

    tree->root = NodeConstruct();

    assert(tree->root);

    tree->root->value = strdup("Unknown object");

    return tree;
}

TreeNode* NodeConstruct()
{
    TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
    assert(node);

    node->value = NULL;
    node->left = NULL;
    node->right = NULL;

    node->parent = NULL;

    return node;
}

void TreeNodeDestroy(TreeNode* node)
{
    if(!node) return;

    TreeNodeDestroy(node->left);
    TreeNodeDestroy(node->right);

    free(node->value);
    free(node);
}

void TreeAdd(TreeNode* node, char* string, TreeAddDir direction)
{
    if(!node) return;
    if(!string) return;

    TreeNode* new_node = NodeConstruct();
    assert(new_node);

    new_node->value = string;
    new_node->parent = node;

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
    assert(tree);

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

    SaveNode(tree->root, file, 0);
    fclose(file);

    return AKINATOR_CORRECT;
}

static void SaveNode(TreeNode* node, FILE* file, size_t rank)
{
    if(!node || !file) return;

    for(size_t tab = 0; tab < rank; tab++)
    {
        fprintf(file, "\t");
    }
    fprintf(file, "(\"%s\"\n", node->value);
    
    SaveNode(node->left, file, rank + 1);
    SaveNode(node->right, file, rank + 1);

    for(size_t tab = 0; tab < rank; tab++)
    {
        fprintf(file, "\t");
    }
    fprintf(file, ")\n");
}

AkinatorState LoadTree(Tree* tree, const char* filename)
{
    if(!tree) return AKINATOR_BUFFER_ERROR;

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

    free(node->value);
    node->value = (char*)calloc(len, sizeof(char));
    strncpy(node->value, buffer, len-1);

    buffer += len;

    buffer = SkipSpaces(buffer);

    if(buffer[0] == ')') return buffer + 1;

    TreeNode* left_node = NodeConstruct();
    node->left = left_node;
    left_node->parent = node;
    buffer = LoadNode(left_node, buffer);

    buffer = SkipSpaces(buffer);

    TreeNode* right_node = NodeConstruct();
    node->right = right_node;
    right_node->parent = node;
    buffer = LoadNode(right_node, buffer);

    buffer = SkipSpaces(buffer);
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
    assert(file);

    fseek(file, 0, SEEK_END);
    size_t filesize = (size_t)ftell(file);
    fseek(file, 0, SEEK_SET);

    return filesize;
}

static char* CreateNewBase()
{
    char* buffer = (char*)calloc(strlen(DEFAULT_LOAD_VALUE) + 1, sizeof(char));
    assert(buffer);

    strcpy(buffer, DEFAULT_LOAD_VALUE);
    return buffer;
}

static char* SkipSpaces(char* string)
{
    assert(string);

    while(*string != '\0' && isspace(*(string)))
    {
        string++;
    }

    return string;
}