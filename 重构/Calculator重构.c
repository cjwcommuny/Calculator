#include <stdio.h>
#include "math.h"
#include "genlib.h"
#include "simpio.h"
#include "queue.h"
#include "stack.h" 
#include <stdlib.h>
#include "mathematic.h"
#include <setjmp.h>

#define ERROR 0
#define SUCCESS 1

#define STRINGSIZE 10

void prompt(void);
void ConvertToPostfix(void);
void process(void);

struct queueADT *OutputQueue;
struct stack *OperatorStack;
struct stack *OperandStack;
jmp_buf ErrorCatcher;

main()
{
    //prompt();
    while (TRUE){
        setjmp(ErrorCatcher);
        fflush(stdin);
        ConvertToPostfix();
        process();
    }
    getchar();
}

void ConvertToPostfix(void)
{
    char *input, *PreChar = NULL;

    OutputQueue = queue_new();
    OperatorStack = NewStack();
    printf("Please input the arithmetic expression:\n");

    while (TRUE) {
        input = GetBlock(sizeof(char)*STRINGSIZE);
        *input = getchar();

        switch (*input) {
            case " ": case "\t":
                break;
            case '0': case '1': case '2': case '3': case '4': case '5' //不宜用switch，因为26个字母...
        }
    }
}