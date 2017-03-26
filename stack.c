#include "stdio.h"
#include "math.h"
#include <string.h>
#include "strlib.h" 
#include "simpio.h" 
#include "stack.h"
#include <setjmp.h>

extern jmp_buf JumpBuffer;

void NewStack(struct stack_node **rootP)
{
	*rootP = NULL;
}

STACKTYPE Pop(struct stack_node **rootP)
{
    STACKTYPE temp;
    struct stack_node *tempP;

    if (StackSize(*rootP) == 0) {
        printf("no element in the stack.\n");
        longjmp(JumpBuffer, 1);
    }
    temp = (*rootP)->value;
    tempP = (*rootP)->next;
    FreeBlock(*rootP);
    *rootP = tempP;
    return temp;
}

void Push(struct stack_node **rootP, STACKTYPE input)
{
    struct stack_node *temp;

    temp = GetBlock(sizeof(struct stack_node));
    temp->value = input;
    temp->next = *rootP;
    *rootP = temp;
}

STACKTYPE Top(struct stack_node **rootP)
{
	if (StackSize(*rootP) == 0) {
        printf("No element.\n");
        longjmp(JumpBuffer, 1);
    }
    return (*rootP)->value;
}

int StackSize(struct stack_node *root)
{
    int size = 0;

    while (root != NULL) {
        size++;
        root = root->next;
    }
    return size;
}

void DestroyStack(struct stack_node **rootP)
{
    while (*rootP != NULL) Pop(rootP);
}


