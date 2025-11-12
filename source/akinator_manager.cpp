#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/akinator_manager.h"
#include "../include/tree_manager.h"

#define TRY(var)                                    \
{                                                   \
    AkinatorState state = var;                      \
    if(state != AKINATOR_CORRECT) return state;     \
}

AkinatorState RunCycle(Tree* tree)
{
    while(true)
    {
        printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
               "\t┃ Welcome to Bibinator! ┃\n"
               "\t┠───────────────────────┨\n"
               "\t┃ Choose gamemode:      ┃\n"
               "\t┃ New database    [n]   ┃\n"
               "\t┃ Play            [p]   ┃\n"
               "\t┃ Description     [d]   ┃\n"
               "\t┃ Comparasion     [c]   ┃\n"
               "\t┃ Quit            [q]   ┃\n"
               "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
        );
        int answer = getchar();
        ClearInput();

        if(answer == 'q')
        {
            break;
        }
        else if(answer == 'n')
        {
            TreeNodeDestroy(tree->root);
            tree->root = NodeConstruct();
            TRY(LoadTree(tree, NULL));
        }
        else
        {
            TreeNodeDestroy(tree->root);
            tree->root = NodeConstruct();
            TRY(LoadTree(tree, TREE_FILENAME));
        }

        TRY(RunAkinator(tree));
        SaveTree(tree, TREE_FILENAME);
    }

    return AKINATOR_CORRECT;
}

AkinatorState RunAkinator(Tree* tree)
{
    assert(tree);

    printf("Think of an object, and I will try to guess it!\n");
    TreeNode* current_node = tree->root;

    assert(current_node);

    while(true)
    {
        if(current_node->left == NULL && current_node->right == NULL)
        {
            printf("Is it %s? [y/n]\n", current_node->value);

            int answer = getchar();
            ClearInput();

            if(answer == 'y')
            {
                printf("Yay! I guessed it right!\n");
                return AKINATOR_CORRECT;
            }
            else
            {
                printf("I give up! What was your object?\n");
                char new_object[INPUT_BUFFER_SIZE + 1];
                scanf("%" XSTR(INPUT_BUFFER_SIZE) "[^\n]", new_object);
                ClearInput();

                printf("Please provide a question that distinguishes %s from %s.\n", 
                       new_object, current_node->value);

                char new_question[INPUT_BUFFER_SIZE + 1];
                scanf("%" XSTR(INPUT_BUFFER_SIZE) "[^\n]", new_question);
                ClearInput();

                char* old_object = strdup(current_node->value);
                
                free(current_node->value);
                current_node->value = strdup(new_question);
                
                TreeAdd(current_node, strdup(new_object), TREE_ADD_RIGHT);   
                TreeAdd(current_node, strdup(old_object), TREE_ADD_LEFT);
                
                free(old_object);
                printf("Thanks! I've learned something new today.\n");
                break;
            }
        }
        else
        {
            printf("%s [y/n]\n", current_node->value);

            int answer = getchar();
            ClearInput();

            current_node = answer == 'y' ? current_node->right : current_node->left;
        }
    }

    return AKINATOR_CORRECT;
}


void ClearInput() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}