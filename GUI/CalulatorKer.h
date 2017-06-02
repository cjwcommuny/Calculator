#ifdef _CalculatorKer_h
#define _CalculatorKer_h

#include <stdio.h>
#include "math.h"
#include "genlib.h"
#include "simpio.h"
#include "queue.h"
#include "stack.h" 
#include <stdlib.h>
#include "mathematic.h"
#include <setjmp.h>

#define MAXSIZE 100
#define STRINGSIZE 10
#define ERROR 0
#define SUCCESS 1

void ker(char STR[]);
int ConvertToPostfix(char STR[]);
void Process(void);
bool CheckAssociation(string op);
int CompareAssociationPriority(char op1, char op2);
int NumOperand(string OperatorNameStr);
double CallFunction(string NameOfFunction, struct stack_node **OperandStackP);
void Output(string str);

queueADT PostfixNotation;
struct stack_node **OperatorStackP;
struct stack_node *OperatorStack;
struct stack_node **OperandStackP;
struct stack_node *OperandStack;
jmp_buf JumpBuffer;
char array[100];

#endif