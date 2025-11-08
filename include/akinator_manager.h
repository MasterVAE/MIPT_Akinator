#ifndef AKINATOR_MANAGER_H_
#define AKINATOR_MANAGER_H_

#include "tree_manager.h"

static const char* TREE_FILENAME = "files/tree.tr";
static const char* DOT_FILENAME = "files/tree.dot";
static const char* DEFAULT_LOAD_VALUE = "(\"Unknown object\")";

static const size_t INPUT_BUFFER_SIZE = 200;

AkinatorState RunAkinator(Tree* tree);

#endif // AKINATOR_MANAGER_H_