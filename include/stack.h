#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>

typedef char* stack_type;

const int SHIELD_START = 0x6BADF00D;
const int SHIELD_END = 0x7BADF00D;
#define ERROR_STREAM stderr
const int STACK_MULTIPLIER = 2;

typedef enum StackError
{
    Verified = 0,
    StackNull = 1 << 1,
    StackOverflow = 1 << 2,
    DataNull = 1 << 3,
    DataCorrupted = 1 << 4,
    CapacityInvalid = 1 << 5,
    StackUnderflow = 1 << 6
} StackError_t;

bool IsError(int error, StackError_t check);

typedef struct Stack
{
    stack_type* data;
    size_t size;
    size_t capacity;
    int err_code;
} Stack_t;


void StackDump(Stack_t* stack);
int StackVerify(Stack_t* stack);
int StackInit(Stack_t* stack, size_t capacity);
int StackPush(Stack_t* stack, stack_type value);
stack_type StackPop(Stack_t* stack, int* err = NULL);
void StackDestroy(Stack_t* stack);

const int shield_size = 1;

#endif // STACK_H_