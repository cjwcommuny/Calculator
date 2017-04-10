/*This library involves operations on stacks*/

#ifndef _stack_h
#define _stack_h

struct stack_node {
    struct stack_node *next;
    void *value;
};

struct stack {
    struct stack_node *head;
    int size;
};

struct stack *NewStack(void);
void *Pop(struct stack *stackP); 
void Push(struct stack *stackP, void *input);
void *Top(struct stack *stackP);
int StackSize(struct stack *stackP);
void DestroyStack(struct stack *stackP);

#endif
