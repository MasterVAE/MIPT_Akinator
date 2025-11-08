#include <stdlib.h>

#include "../include/tree_manager.h"
#include "../include/tree_logger.h"
#include "../include/akinator_manager.h"

static void TreeDotDumpStart(Tree* tree, FILE* file);
static void TreeDotDumpEnd(Tree* tree, FILE* file);

void PrintNode(TreeNode* node)
{
    if (node == NULL)   return;

    PrintNode(node->left);

    printf("%s ", node->string);

    PrintNode(node->right);
}

void DotPrintNode(TreeNode* node, FILE* file, int rank)
{
    if (node == NULL)   return;

    fprintf(file, "subgraph cluster_%p                                     \n", node);
    fprintf(file, "{                                                       \n");
    fprintf(file, "\tstyle=filled;                                         \n");
    fprintf(file, "\trank=%d;                                              \n", rank);
    fprintf(file, "\tfillcolor=lavender;                                   \n");
    fprintf(file, "\t\"%p_address\"[label=\"%p\",fillcolor=\"coral\"];     \n", node, node);
    fprintf(file, "\t\"%p_value\"[label=\"%s\",fillcolor=\"salmon\"];      \n", node, node->string);
    fprintf(file, "\tsubgraph \"%p_bottom\"                                \n", node);
    fprintf(file, "\t{                                                     \n");
    fprintf(file, "\t\t style=invis;                                       \n");
    fprintf(file, "\t\t\"%p_left\"[label=\"%p\",fillcolor=\"lightblue\"];  \n",node,  node->left);
    fprintf(file, "\t\t\"%p_right\"[label=\"%p\",fillcolor=\"lightgreen\"];\n", node, node->right);
    fprintf(file, "\t}                                                     \n");
    fprintf(file, "\t\"%p_address\"->\"%p_value\"[style=invis,weight=10];  \n", node, node);
    fprintf(file, "\t\"%p_value\"->\"%p_left\"[style=invis,weight=10];     \n", node, node);
    fprintf(file, "\t\"%p_value\"->\"%p_right\"[style=invis,weight=10];    \n", node , node);
    fprintf(file, "}                                                       \n");

    if (node->left != NULL)
    {
        fprintf(file, "\"%p_left\" -> \"%p_address\";\n", node, node->left);
        DotPrintNode(node->left, file, rank + 1);
    }
    if (node->right != NULL)
    {
        fprintf(file, "\"%p_right\" -> \"%p_address\";\n", node, node->right);
        DotPrintNode(node->right, file, rank + 1);
    }
}

void TreeDotDumpStart(Tree* tree, FILE* file)
{
    fprintf(file, "digraph G {"
                  "\t node [shape=plaintext, style=\"filled\"];\n"
                  "\t rankdir=TB;\n");
}

void TreeDotDumpEnd(Tree* tree, FILE* file)
{
    fprintf(file, "}\n");
}

void TreeDotDump(Tree* tree, const char* filename)
{
    FILE* file = fopen(filename, "w+");

    TreeDotDumpStart(tree, file);
    DotPrintNode(tree->root, file, 0);
    TreeDotDumpEnd(tree, file);

    fclose(file);

    char command[100] = {};
    snprintf(command, 100, "dot -Tpng %s -o files/tree.png", DOT_FILENAME);
    system(command);

}