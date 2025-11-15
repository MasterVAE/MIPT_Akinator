#include <stdlib.h>
#include <string.h>

#include "stack.h"

void ErrorParser(int error);

bool IsError(int error, StackError check)
{
    return error & check;
}

void ErrorParser(int error)
{
    if(IsError(error, StackNull))       fprintf(ERROR_STREAM, "Error: stack NULL\n");
    if(IsError(error, CapacityInvalid)) fprintf(ERROR_STREAM, "Error: capacity invalid\n");
    if(IsError(error, DataNull))        fprintf(ERROR_STREAM, "Error: data NULL\n");
    if(IsError(error, StackOverflow))   fprintf(ERROR_STREAM, "Error: stack overflow\n");
    if(IsError(error, StackUnderflow))  fprintf(ERROR_STREAM, "Error: stack underflow\n");
    if(IsError(error, DataCorrupted))   fprintf(ERROR_STREAM, "Error: data corrupted\n");
}

int StackVerify(Stack_t* stack)
{
    if(stack == NULL) return StackNull;
    stack->err_code = Verified;
    if(stack->capacity == 0) stack->err_code |= CapacityInvalid;
    if(stack->size > stack->capacity) stack->err_code |= StackOverflow;
    if(stack->data == NULL) return stack->err_code |= DataNull;

    return stack->err_code;
}

int StackInit(Stack_t* stack, size_t capacity)
{
    if(stack == NULL) return StackNull;

    if(capacity == 0) capacity = 1;
    stack->capacity = capacity;
    stack->size = 0;
    stack->data = (stack_type*)calloc(capacity + 2 * shield_size, sizeof(stack_type));
    stack->err_code = 0;

    return StackVerify(stack);
}

int StackPush(Stack_t* stack, stack_type value)
{
    int err = StackVerify(stack); if(err != 0) return err;

    if(stack->size < stack->capacity)
    {
        stack->data[1 * shield_size + stack->size++] = value;
        err = StackVerify(stack); if(err != 0) return err;

        return Verified;
    }

    stack->capacity *= STACK_MULTIPLIER;
    stack_type* new_data = (stack_type*)realloc(stack->data, 
                                        (stack->capacity + 2 * shield_size) * sizeof(stack_type));
    if(!new_data) free(stack->data);
    stack->data = new_data;

    err = StackVerify(stack); if(err != 0) return err;

    stack->data[1 * shield_size + stack->size++] = value;

    err = StackVerify(stack); if(err != 0) return err;

    return Verified;
}

stack_type StackPop(Stack_t* stack, int* err)
{
    int error = StackVerify(stack);
    if(error != 0)
    {
        if(err != NULL) *err = error;
        return 0;
    }
    if(stack->size > 0)
    {
        if(err != NULL) *err = Verified;
        stack->size--;
        return stack->data[1 * shield_size + stack->size];
    }
    if(err != NULL) *err = StackUnderflow;
    
    return 0;
}

void StackDestroy(Stack_t* stack)
{
    if(stack == NULL) return;
    free(stack->data);
    memset(stack, 0, sizeof(Stack_t));
}

void StackDump(Stack_t* stack)
{
        fprintf(ERROR_STREAM,"\n- - - STACK dumping - - - \n");
    ErrorParser(stack->err_code);
    if(stack == NULL)
    {   
        fprintf(ERROR_STREAM, "NULL Stack\n");
        return;
    }
        
        fprintf(ERROR_STREAM, "CAPACITY: %lu\n", stack->capacity);
        fprintf(ERROR_STREAM, "SIZE: %lu\n\n", stack->size);

    if(stack->data == NULL)
    {
        fprintf(ERROR_STREAM, "NULL DATA\n");
        return;
    }
        fprintf(ERROR_STREAM, "DATA: \n\n");
    for(size_t i = 1; i < stack->capacity + 1 * shield_size; i++)
    {
        if(i <= stack->size)
        {
        fprintf(ERROR_STREAM, " *[%lu] %p\n", i, stack->data[i]);
        }
        else
        {
        fprintf(ERROR_STREAM, "  [%lu] %p\n", i, stack->data[i]);
        }
    }

}