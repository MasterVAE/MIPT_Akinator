#include <stdlib.h>

#include "../include/tree_manager.h"
#include "../include/tree_logger.h"
#include "../include/akinator_manager.h"

int main()
{
    Tree* tree = TreeConstruct();

    DO(RunCycle(tree));

    TreeDotDump(tree, DOT_FILENAME, DotPrintString);
    TreeDestroy(tree);

    return 0;
}