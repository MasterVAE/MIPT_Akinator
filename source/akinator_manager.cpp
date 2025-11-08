#include <string.h>
#include <stdlib.h>

#include "../include/akinator_manager.h"
#include "../include/tree_manager.h"

AkinatorState RunAkinator(Tree* tree)
{
    printf("Think of an object, and I will try to guess it!\n");
    TreeNode* current_node = tree->root;

    while(true)
    {
        if(current_node->left == NULL && current_node->right == NULL)
        {
            printf("Is it %s? [y/n]\n", current_node->string);
            char answer;
            scanf("%c", &answer);

            if(answer == 'y')
            {
                printf("Yay! I guessed it right!\n");
            }
            else
            {
                printf("I give up! What was your object?\n");
                char new_object[INPUT_BUFFER_SIZE];
                scanf("%s", new_object);

                printf("Please provide a question that distinguishes %s from %s.\n", new_object, current_node->string);
                char new_question[INPUT_BUFFER_SIZE];
                scanf("%s", new_question);

                TreeAdd(current_node, strdup(current_node->string), TREE_ADD_LEFT);
                TreeAdd(current_node, strdup(new_object), TREE_ADD_RIGHT);

                current_node->string = strdup(new_question);

                printf("Thanks! I've learned something new today.\n");
            }
            break;
        }
        else
        {
            printf("%s [y/n]\n", current_node->string);
            char answer;
            scanf("%c", &answer);

            current_node = answer == 'y' ? current_node->right : current_node->left;
        }
    }

    return AKINATOR_CORRECT;
}

