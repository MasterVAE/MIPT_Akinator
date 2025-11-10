#include <stdlib.h>

#include "../include/tree_manager.h"
#include "../include/tree_logger.h"
#include "../include/akinator_manager.h"

int main()
{
    Tree* tree = TreeConstruct();

    printf("Create new base?\n");
    int answer = getchar();
    getchar();

    DO(LoadTree(tree, answer == 'y' ? NULL : TREE_FILENAME));

    DO(RunAkinator(tree));

    //PrintNode(tree->root);
    TreeDotDump(tree, DOT_FILENAME);

    DO(SaveTree(tree, TREE_FILENAME));
    TreeDestroy(tree);

    return 0;
}