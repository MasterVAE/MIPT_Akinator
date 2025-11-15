#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stack.h>

#include "../include/akinator_manager.h"
#include "../include/tree_manager.h"

#define TRY(var)                                    \
{                                                   \
    AkinatorState state = var;                      \
    if(state != AKINATOR_CORRECT) return state;     \
}

#define CLEAR "\033[H\033[2J"

static const size_t WINDOW_WIDTH = 23;

static void ClearInput();
static AkinatorState RunAkinator(Tree* tree);
static AkinatorState DescriptionAkinator(Tree* tree);
static AkinatorState ComparatorAkinator(Tree* tree);
static void PrintCommon(Stack_t* parent_stack1, Stack_t* parent_stack2);
static void PrintDifferent(Stack_t* parent_stack1, Stack_t* parent_stack2, 
                           TreeNode* node1, TreeNode* node2);
static void CompareNodes(TreeNode* node1, TreeNode* node2);
static void CreateParentStack(Stack_t* stack, TreeNode* node);
static void InsertText(const char* text, size_t count);
static void AnyButtonExit();

AkinatorState RunCycle(Tree* tree)
{
    assert(tree);

    while(true)
    {
        printf(CLEAR);

        printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
               "\t┃ Welcome to Bibinator! ┃\n"
               "\t┠───────────────────────┨\n"
               "\t┃ Choose gamemode:      ┃\n"
               "\t┃ Load database   [%c]   ┃\n"
               "\t┃ Save database   [%c]   ┃\n"
               "\t┃ Play            [%c]   ┃\n"
               "\t┃ Description     [%c]   ┃\n"
               "\t┃ Comparasion     [%c]   ┃\n"
               "\t┃ Quit            [%c]   ┃\n"
               "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n",
        INPUT_LOAD, INPUT_SAVE, INPUT_PLAY, INPUT_DESCRIPTION, INPUT_COMPARATOR, INPUT_QUIT);
        int answer = getchar();
        ClearInput();
        
        if(answer == INPUT_QUIT)
        {
            break;
        }
        else if(answer == INPUT_LOAD)
        {
            printf(CLEAR);
            printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                   "\t┃    Input filename     ┃\n"
                   "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n"
            );

            char filename[INPUT_BUFFER_SIZE + 1];
            scanf("%" XSTR(INPUT_BUFFER_SIZE) "[^\n]", filename);
            ClearInput();

            TreeNodeDestroy(tree->root);
            tree->root = NodeConstruct();
            TRY(LoadTree(tree, filename));
        }
        else if(answer == INPUT_SAVE)
        {
            printf(CLEAR);
            printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                   "\t┃    Input filename     ┃\n"
                   "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n"
            );

            char filename[INPUT_BUFFER_SIZE + 1];
            scanf("%" XSTR(INPUT_BUFFER_SIZE) "[^\n]", filename);
            ClearInput();

            TRY(SaveTree(tree, filename));
        }
        else if(answer == INPUT_PLAY)
        {
            TRY(RunAkinator(tree));
        }
        else if(answer == INPUT_DESCRIPTION)
        {
            TRY(DescriptionAkinator(tree));
        }
        else if(answer == INPUT_COMPARATOR)
        {
            TRY(ComparatorAkinator(tree));
        }
    }

    return AKINATOR_CORRECT;
}

static AkinatorState RunAkinator(Tree* tree)
{
    assert(tree);

    TreeNode* current_node = tree->root;
    assert(current_node);

    while(true)
    {
        if(current_node->left == NULL && current_node->right == NULL)
        {
            printf(CLEAR);
            printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                   "\t┃         Is it         ┃\n"
                   "\t┃"                        );
            InsertText(current_node->value, WINDOW_WIDTH);
            printf(                          "┃\n"
                   "\t┃     [yes]    [no]     ┃\n"
                   "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
            );

            char answer[INPUT_BUFFER_SIZE + 1];
            scanf("%" XSTR(INPUT_BUFFER_SIZE) "[^\n]", answer);
            ClearInput();

            if(!strcmp(answer, INPUT_RIGHT))
            {
                printf(CLEAR);
                printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                       "\t┃         Yay!          ┃\n"
                       "\t┃  I guessed it right!  ┃\n"
                       "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
                );
                
                AnyButtonExit();

                return AKINATOR_CORRECT;
            }
            else if(!strcmp(answer, INPUT_WRONG)) 
            {
                printf(CLEAR);
                printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                       "\t┃      I give up!       ┃\n"
                       "\t┃ What was your object? ┃\n"
                       "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
                );

                char new_object[INPUT_BUFFER_SIZE + 1];
                scanf("%" XSTR(INPUT_BUFFER_SIZE) "[^\n]", new_object);
                ClearInput();

                printf(CLEAR);
                printf(CLEAR);
                printf("    \t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                           "\t┃     Please provide    ┃\n"
                           "\t┃    a question that    ┃\n"
                           "\t┃     distinguishes     ┃\n\t┃");
                InsertText(new_object, WINDOW_WIDTH);
                printf("┃ \n\t┃         from          ┃\n\t┃");
                InsertText(current_node->value, WINDOW_WIDTH);
                printf("┃ \n\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n");

                char new_question[INPUT_BUFFER_SIZE + 1];
                scanf("%" XSTR(INPUT_BUFFER_SIZE) "[^\n]", new_question);
                ClearInput();

                char* old_object = strdup(current_node->value);
                assert(old_object);
                
                free(current_node->value);
                current_node->value = strdup(new_question);
                
                TreeAdd(current_node, strdup(new_object), TREE_ADD_RIGHT);   
                TreeAdd(current_node, strdup(old_object), TREE_ADD_LEFT);
                
                free(old_object);

                printf(CLEAR);
                printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                       "\t┃        Thanks!        ┃\n"
                       "\t┃I've learned something ┃\n"
                       "\t┃       new today.      ┃\n"
                       "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
                );
                
                AnyButtonExit();

                break;
            }
            else
            {
                printf(CLEAR);
                printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                       "\t┃        Sorry?         ┃\n"
                       "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
                );
                
                AnyButtonExit();

                continue;
            }
        }
        else
        {
            printf(CLEAR);
            printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                   "\t┃"                        );
            InsertText(current_node->value, WINDOW_WIDTH);
            printf(                          "┃\n"
                   "\t┃     [yes]    [no]     ┃\n"
                   "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
            );            


            char answer[INPUT_BUFFER_SIZE + 1];
            scanf("%" XSTR(INPUT_BUFFER_SIZE) "[^\n]", answer);
            ClearInput();

            if(!strcmp(answer, INPUT_RIGHT))
            {
                current_node = current_node->right;
            }
            else if(!strcmp(answer, INPUT_WRONG))
            {
                current_node = current_node->left;
            }
            else
            {
                printf(CLEAR);
                printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                       "\t┃        Sorry?         ┃\n"
                       "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
                );
                
                AnyButtonExit();

                continue;
            }
        }
    }

    return AKINATOR_CORRECT;
}

static AkinatorState DescriptionAkinator(Tree* tree)
{
    assert(tree);

    printf(CLEAR);
    printf("    \t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
               "\t┃      Write object,    ┃\n"
               "\t┃    and I will try     ┃\n"
               "\t┃     to make it's      ┃\n"
               "\t┃      description      ┃\n"
               "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
        );

    char node_name[INPUT_BUFFER_SIZE + 1];
    scanf("%" XSTR(INPUT_BUFFER_SIZE) "[^\n]", node_name);
    ClearInput();

    TreeNode* node = FindNode(tree->root, node_name);
    if(node)
    {   
        Stack_t* parent_stack = (Stack_t*)calloc(1, sizeof(Stack_t));
        StackInit(parent_stack, 1);

        CreateParentStack(parent_stack, node);

        printf(CLEAR);
        printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
               "\t┃");
        InsertText(node->value, WINDOW_WIDTH);
        printf("┃\n");
        printf("\t┠───────────────────────┨\n");
        while(parent_stack->size > 1)
        {
            TreeNode* stack_node = StackPop(parent_stack);
            TreeNode* next_node = StackPop(parent_stack);
            
            if(stack_node->right == next_node)
            {
                printf("\t┃-> ");
                InsertText(stack_node->value, WINDOW_WIDTH - 3);
                printf("┃\n");
            }
            else
            {
                printf("\t┃-> Not ");
                InsertText(stack_node->value, WINDOW_WIDTH - 7);
                printf("┃\n");
            }

            StackPush(parent_stack, next_node);
        }
        printf("\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n");

        StackDestroy(parent_stack);
        free(parent_stack);

        AnyButtonExit();
    }
    else
    {
        printf(CLEAR);
        printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
               "\t┃       Not found       ┃\n"
               "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
        );

        AnyButtonExit();
    }

    return AKINATOR_CORRECT;
}

static AkinatorState ComparatorAkinator(Tree* tree)
{
    assert(tree);

    printf(CLEAR);
    printf("    \t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
               "\t┃     Write objects,    ┃\n"
               "\t┃    and I will try     ┃\n"
               "\t┃    to compare them    ┃\n"
               "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
          );

    char node_name1[INPUT_BUFFER_SIZE + 1];
    scanf("%" XSTR(INPUT_BUFFER_SIZE) "[^\n]", node_name1);
    ClearInput();

    char node_name2[INPUT_BUFFER_SIZE + 1];
    scanf("%" XSTR(INPUT_BUFFER_SIZE) "[^\n]", node_name2);
    ClearInput();

    TreeNode* node1 = FindNode(tree->root, node_name1);
    TreeNode* node2 = FindNode(tree->root, node_name2);
    if(node1 && node2)
    {  
        CompareNodes(node1, node2);

        AnyButtonExit();
    }
    else
    {
        printf(CLEAR);
        printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
               "\t┃       Not found       ┃\n"
               "\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n"
        );
        
        AnyButtonExit();
    }

    return AKINATOR_CORRECT;
}

static void CompareNodes(TreeNode* node1, TreeNode* node2)
{
    assert(node1);
    assert(node2);

    Stack_t* parent_stack1 = (Stack_t*)calloc(1, sizeof(Stack_t));
    Stack_t* parent_stack2 = (Stack_t*)calloc(1, sizeof(Stack_t));

    StackInit(parent_stack1, 1);
    StackInit(parent_stack2, 1);

    CreateParentStack(parent_stack1, node1);
    CreateParentStack(parent_stack2, node2);

    printf(CLEAR);
    printf("\t┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"
           "\t┃");
    InsertText(node1->value, WINDOW_WIDTH);
    printf("┃\n");
    printf("\t┠───────────────────────┨\n\t┃");
    InsertText(node2->value, WINDOW_WIDTH);
    printf(  "┃\n"
           "\t┠───────────────────────┨\n"
           "\t┃        COMMON         ┃\n");

    PrintCommon(parent_stack1, parent_stack2);

    printf("\t┠───────────────────────┨\n"
           "\t┃       DIFFERENT       ┃\n");

    PrintDifferent(parent_stack1, parent_stack2, node1, node2);

    printf("\t┗━━━━━━━━━━━━━━━━━━━━━━━┛\n");

    StackDestroy(parent_stack1);
    free(parent_stack1);
    StackDestroy(parent_stack2);
    free(parent_stack2);
}

static void PrintCommon(Stack_t* parent_stack1, Stack_t* parent_stack2)
{
    assert(parent_stack1);
    assert(parent_stack2);

    while(parent_stack1->size > 1 && parent_stack2->size > 1)
    {
        TreeNode* stack_node1 = StackPop(parent_stack1);
        TreeNode* next_node1 = StackPop(parent_stack1);

        TreeNode* stack_node2 = StackPop(parent_stack2);
        TreeNode* next_node2 = StackPop(parent_stack2);
        
        if(stack_node1->right == next_node1 && stack_node2->right == next_node2 
                                            && !strcmp(stack_node1->value, stack_node2->value))
        {
            printf("\t┃-> ");
            InsertText(stack_node1->value, WINDOW_WIDTH - 3);
            printf("┃\n");
        }
        else if(stack_node1->left == next_node1 && stack_node2->left == next_node2
                                                && !strcmp(stack_node1->value, stack_node2->value))
        {
            printf("\t┃-> Not ");
            InsertText(stack_node1->value, WINDOW_WIDTH - 7);
            printf("┃\n");
        }
        else
        {
            StackPush(parent_stack1, next_node1);
            StackPush(parent_stack2, next_node2);

            StackPush(parent_stack1, stack_node1);
            StackPush(parent_stack2, stack_node2);

            break;
        }

        StackPush(parent_stack1, next_node1);
        StackPush(parent_stack2, next_node2);
    }
}

static void PrintDifferent(Stack_t* parent_stack1, Stack_t* parent_stack2, 
                           TreeNode* node1, TreeNode* node2)
{
    assert(parent_stack1);
    assert(parent_stack2);
    assert(node1);
    assert(node2);

    printf("\t┃");
    InsertText(node1->value, WINDOW_WIDTH);
    printf("┃\n");
    while(parent_stack1->size > 1)
    {
        TreeNode* stack_node = StackPop(parent_stack1);
        TreeNode* next_node = StackPop(parent_stack1);
        
        if(stack_node->right == next_node)
        {
            printf("\t┃-> ");
            InsertText(stack_node->value, WINDOW_WIDTH - 3);
            printf("┃\n");
        }
        else
        {
            printf("\t┃-> Not ");
            InsertText(stack_node->value, WINDOW_WIDTH - 7);
            printf("┃\n");
        }

        StackPush(parent_stack1, next_node);
    }

    printf("\t┃");
    InsertText(node2->value, WINDOW_WIDTH);
    printf("┃\n");
    while(parent_stack2->size > 1)
    {
        TreeNode* stack_node = StackPop(parent_stack2);
        TreeNode* next_node = StackPop(parent_stack2);
        
        if(stack_node->right == next_node)
        {
            printf("\t┃-> ");
            InsertText(stack_node->value, WINDOW_WIDTH - 3);
            printf("┃\n");
        }
        else
        {
            printf("\t┃-> Not ");
            InsertText(stack_node->value, WINDOW_WIDTH - 7);
            printf("┃\n");
        }

        StackPush(parent_stack2, next_node);
    }
}


static void ClearInput() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

TreeNode* FindNode(TreeNode* node, const char* value)
{
    if(!node) return NULL;
    assert(value);

    if(!strcmp(value, node->value)) return node;

    TreeNode* node2 = FindNode(node->left, value);
    if(node2) return node2;

    node2 = FindNode(node->right, value);
    if(node2) return node2;

    return NULL;
}

static void CreateParentStack(Stack_t* stack, TreeNode* node)
{
    assert(stack);
    if(!node) return;

    StackPush(stack, node);

    CreateParentStack(stack, node->parent);
}

static void InsertText(const char* text, size_t count)
{
    assert(text);

    size_t len = strlen(text);

    size_t spaces_before = (count - len)/2;
    size_t spaces_after = count - len - spaces_before;

    for(size_t i = 0; i < spaces_before; i++)
    {
        printf(" ");
    }

    printf("%s", text);

    for(size_t i = 0; i < spaces_after; i++)
    {
        printf(" ");
    }
}

static void AnyButtonExit()
{
    printf("\nPress any button to continue\n");
    getchar();
    ClearInput();
}