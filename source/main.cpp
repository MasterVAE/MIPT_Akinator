#include <stdlib.h>

#include "tree_manager.h"
#include "tree_logger.h"
#include "akinator_manager.h"

int main()
{
    Tree* tree = TreeConstruct();

    DO(RunCycle(tree));
// FIXME - dot and png to another papka
    TreeDotDump(tree, DOT_FILENAME, DotPrintString);
    TreeDestroy(tree);

    return 0;
}