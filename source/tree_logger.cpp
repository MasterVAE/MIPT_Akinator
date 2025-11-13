#include <stdlib.h>
#include <assert.h>

#include "../include/tree_manager.h"
#include "../include/tree_logger.h"
#include "../include/akinator_manager.h"

static void TreeDotDumpStart(FILE* file);
static void TreeDotDumpEnd(FILE* file);

static void DotPrintNode(TreeNode* node, FILE* file, int rank);

void PrintNode(TreeNode* node)
{
    if(!node) return;

    PrintNode(node->left);

    printf("%s ", node->value);

    PrintNode(node->right);
}

static void DotPrintNode(TreeNode* node, FILE* file, int rank)
{
    assert(node);

    fprintf(file, "subgraph cluster_%p                                     \n", node);
    fprintf(file, "{                                                       \n");
    fprintf(file, "\tstyle=filled;                                         \n");
    fprintf(file, "\trank=%d;                                              \n", rank);
    fprintf(file, "\tfillcolor=lavender;                                   \n");
    fprintf(file, "\t\"%p_address\"[label=\"%p\",fillcolor=\"lavender\"];  \n", node, node);
    fprintf(file, "\t\"%p_value\"[label=\"%s\",fillcolor=\"lightblue\"];   \n", node, node->value);
    fprintf(file, "\tsubgraph \"%p_bottom\"                                \n", node);
    fprintf(file, "\t{                                                     \n");
    fprintf(file, "\t\t style=invis;                                       \n");
    if(node->left) {
    fprintf(file, "\t\t\"%p_left\"[label=\"%p\",fillcolor=\"coral\"];      \n",node,  node->left);}

    if(node->right) {
    fprintf(file, "\t\t\"%p_right\"[label=\"%p\",fillcolor=\"lightgreen\"];\n", node, node->right);}
    
    fprintf(file, "\t}                                                     \n");
    fprintf(file, "\t\"%p_address\"->\"%p_value\"[style=invis,weight=10];  \n", node, node);
    
    if(node->left) {
    fprintf(file, "\t\"%p_value\"->\"%p_left\"[style=invis,weight=10];     \n", node, node);}

    if(node->right) {
    fprintf(file, "\t\"%p_value\"->\"%p_right\"[style=invis,weight=10];    \n", node , node);}
    
    fprintf(file, "}                                                       \n");

    if (node->left)
    {
        fprintf(file, "\"%p_left\" -> \"%p_address\";\n", node, node->left);
        DotPrintNode(node->left, file, rank + 1);
    }
    if (node->right)
    {
        fprintf(file, "\"%p_right\" -> \"%p_address\";\n", node, node->right);
        DotPrintNode(node->right, file, rank + 1);
    }
}

static void TreeDotDumpStart(FILE* file)
{
    assert(file);

    fprintf(file, "digraph G {"
                  "\t node [shape=plaintext, style=\"filled\"];\n"
                  "\t rankdir=TB;\n");
}

static void TreeDotDumpEnd(FILE* file)
{
    assert(file);

    fprintf(file, "}\n");
}

void TreeDotDump(Tree* tree, const char* filename)
{ 
    assert(tree);
    assert(filename);

    FILE* file = fopen(filename, "w+");

    TreeDotDumpStart(file);
    DotPrintNode(tree->root, file, 0);
    TreeDotDumpEnd(file);

    fclose(file);

    char command[INPUT_BUFFER_SIZE] = {};
    snprintf(command, INPUT_BUFFER_SIZE, "dot -Tpng %s -o files/tree.png", DOT_FILENAME);
    system(command);

}