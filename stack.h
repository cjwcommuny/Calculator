/*This library involves operations on stacks*/

#ifndef _stack_h
#define _stack_h

#define STACKTYPE char *

struct stack_node {
    struct stack_node *next;
    STACKTYPE value;
};

void NewStack(struct stack_node **rootP);
STACKTYPE Pop(struct stack_node **rootP); 
void Push(struct stack_node **rootP, STACKTYPE input);
STACKTYPE Top(struct stack_node **rootP);
int StackSize(struct stack_node *root);
void DestroyStack(struct stack_node **rootP);

#endif
