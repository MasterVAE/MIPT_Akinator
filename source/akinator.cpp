#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/tree_manager.h"
#include "../include/tree_logger.h"

int main()
{
    Tree* tree = TreeConstruct();
    tree->dotfile = fopen(DOT_FILENAME, "w+");

    srand((unsigned)time(NULL));
    for(size_t i = 0; i < 10; i++)
    {
        TreeAdd(tree, rand() % 100);
    }

    PrintNode(tree->root);
    TreeDotDump(tree);
    TreeDestroy(tree);
}