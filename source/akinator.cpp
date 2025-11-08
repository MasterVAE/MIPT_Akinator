#include <stdlib.h>

#include "../include/tree_manager.h"
#include "../include/tree_logger.h"
#include "../include/akinator_manager.h"

int main()
{
    Tree* tree = TreeConstruct();
    LoadTree(tree, TREE_FILENAME);

    RunAkinator(tree);

    //PrintNode(tree->root);
    TreeDotDump(tree, DOT_FILENAME);

    SaveTree(tree, TREE_FILENAME);
    TreeDestroy(tree);
}