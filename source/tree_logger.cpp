#include <stdlib.h>

#include "../include/tree_manager.h"
#include "../include/tree_logger.h"

void PrintNode(TreeNode* node)
{
    if (node == NULL)   return;

    PrintNode(node->left);

    printf("%d ", node->value);

    PrintNode(node->right);
}

void DotPrintNode(TreeNode* node, FILE* file, int rank)
{
    if (node == NULL)   return;

    fprintf(file, "subgraph cluster_%p                                      \n", node);
    fprintf(file, "{                                                        \n");
    fprintf(file, "\t style=filled;                                         \n");
    fprintf(file, "\t rank=%d;                                              \n", rank);
    fprintf(file, "\t fillcolor=lavender;                                   \n");
    fprintf(file, "\t \"%p_address\"[label=\"%p\",fillcolor=\"coral\"];     \n", node, node);
    fprintf(file, "\t \"%p_value\"[label=\"%d\",fillcolor=\"salmon\"];      \n", node, node->value);
    fprintf(file, "\t subgraph \"%p_bottom\"                                \n", node);
    fprintf(file, "\t {                                                     \n");
    fprintf(file, "\t\t style=invis;                                        \n");
    fprintf(file, "\t\t \"%p_left\"[label=\"%p\",fillcolor=\"lightblue\"];  \n",node,  node->left);
    fprintf(file, "\t\t \"%p_right\"[label=\"%p\",fillcolor=\"lightgreen\"];\n", node, node->right);
    fprintf(file, "\t }                                                     \n");
    fprintf(file, "\t \"%p_address\"->\"%p_value\"[style=invis,weight=10];  \n", node, node);
    fprintf(file, "\t \"%p_value\"->\"%p_left\"[style=invis,weight=10];     \n", node, node);
    fprintf(file, "\t \"%p_value\"->\"%p_right\"[style=invis,weight=10];    \n", node , node);
    fprintf(file, "}                                                        \n");

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

void TreeDotDumpStart(Tree* tree)
{
    fprintf(tree->dotfile, "digraph G {"
                           "\tnode [shape=plaintext, style=\"filled\"];\n"
                           "\trankdir=TB;\n");
}

void TreeDotDumpEnd(Tree* tree)
{
    fprintf(tree->dotfile, "}\n");
}

void TreeDotDump(Tree* tree)
{
    TreeDotDumpStart(tree);
    DotPrintNode(tree->root, tree->dotfile, 0);
    TreeDotDumpEnd(tree);

    fclose(tree->dotfile);
    char command[100] = {};
    snprintf(command, 100, "dot -Tpng %s -o files/tree.png", DOT_FILENAME);
    system(command);
}