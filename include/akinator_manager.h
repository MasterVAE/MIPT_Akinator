#ifndef AKINATOR_MANAGER_H_
#define AKINATOR_MANAGER_H_

#include "tree_manager.h"

static const char* const TREE_FILENAME = "files/tree.tr";
static const char* const DOT_FILENAME = "files/tree.dot";
static const char* const DEFAULT_LOAD_VALUE = "(\"Unknown object\")";

#define INPUT_BUFFER_SIZE 200

#define STR(x) #x
#define XSTR(x) STR(x)

AkinatorState RunCycle(Tree* tree);
TreeNode* FindNode(TreeNode* node, const char* value);
void DotPrintString(FILE* file, TreeNode* node); 

#define DO(state)                                       \
{                                                       \
    AkinatorState state_var = state;                    \
    if(state_var != AKINATOR_CORRECT)                   \
    {                                                   \
        printf("Error occurred: %d\n", state_var);      \
        TreeDotDump(tree, DOT_FILENAME, DotPrintString);\
        TreeDestroy(tree);                              \
        return state_var;                               \
    }                                                   \
}

enum InputKeys
{
    INPUT_QUIT = 'q',
    INPUT_LOAD = 'l',
    INPUT_SAVE = 's',
    INPUT_PLAY = 'p',
    INPUT_DESCRIPTION = 'd',
    INPUT_COMPARATOR = 'c'
};

static const char* const INPUT_RIGHT = "yes";
static const char* const INPUT_WRONG = "no";

#endif // AKINATOR_MANAGER_H_