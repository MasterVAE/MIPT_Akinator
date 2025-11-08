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
            char answer[2];
            scanf("%1s", answer);
            getchar();

            if(!strcmp(answer, "y"))
            {
                printf("Yay! I guessed it right!\n");
                return AKINATOR_CORRECT;
            }
            else
            {
                printf("I give up! What was your object?\n");
                char new_object[INPUT_BUFFER_SIZE];
                scanf("%[^\n]", new_object);
                getchar();

                printf("Please provide a question that distinguishes %s from %s.\n", 
                       new_object, current_node->string);
                char new_question[INPUT_BUFFER_SIZE];
                scanf("%[^\n]", new_question);
                getchar();

                char* old_object = strdup(current_node->string);
                
                free(current_node->string);
                current_node->string = strdup(new_question);
                
                TreeAdd(current_node, strdup(new_object), TREE_ADD_RIGHT);   
                TreeAdd(current_node, strdup(old_object), TREE_ADD_LEFT);
                
                free(old_object);
                printf("Thanks! I've learned something new today.\n");
                break;
            }
        }
        else
        {
            printf("%s [y/n]\n", current_node->string);
            char answer[2];
            scanf("%1s", answer);
            getchar();

            current_node = !strcmp(answer, "y") ? current_node->right : current_node->left;
        }
    }

    return AKINATOR_CORRECT;
}
