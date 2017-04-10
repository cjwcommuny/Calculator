#include "stdio.h"
#include "math.h"
#include <string.h>
#include "strlib.h" 
#include "simpio.h" 
#include "stack.h"



struct stack *NewStack(void)
{
    struct stack *stackP;

    stackP = GetBlock(sizeof(struct stack));
    stackP->head = NULL;
    stackP->size = 0;
}

void *Pop(struct stack *stackP)
{
    void *value;
    struct stack_node *temp;

    if (!StackSize(stackP)) Error("no element in the stack.\n");
    else {
        value = stackP->head->value;
        stackP->size--;
        temp = stackP->head->next;
        free(stackP->head);
        stackP->head = temp;
        return value;
    }
}

void Push(struct stack *stackP, void *input)
{
    struct stack_node *new_node;

    new_node = GetBlock(sizeof(struct stack_node));
    new_node->value = input;
    new_node->next = stackP->head;
    stackP->head = new_node;
    stackP->size++;
}

void *Top(struct stack *stackP)
{
    if (!StackSize(stackP)) Error("No element.\n");
    return stackP->head->value;
}

int StackSize(struct stack *stackP)
{
    return stackP->size;
}

void DestroyStack(struct stack *stackP)
{
    while (StackSize(stackP)) Pop(stackP);
}


