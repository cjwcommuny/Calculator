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


void Prompt(void);
int ConvertToPostfix(void);
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

main()
{
    Prompt();
    while (TRUE){
        setjmp(JumpBuffer);
        fflush(stdin);
        if (ConvertToPostfix()) Process();
    }
    getchar(); 
}

void Prompt(void){
	printf("This is a CALCULATOR used to calculate the result of the formula.\n\n");
	printf("OPERATORS supported :\n+, -, *, ^(power), /\n\n");
	printf("FUNCTIONS supported:\nsin, cos, tan, arcsin, arccos, arctan, lg, ln, ! (factorial), exp, abs and sqrt.\n");
	printf("NOTE: The program also supports the functions of several variables, for example log(base, antilog).\n\n");
	printf("CONSTANTS supported:\nthe symbols pi, pI, Pi and PI represent the constant 3.14159 \nthe symbol e represents the constant 2.7128.\n");
	printf("NOTE: When you use funtions such as sin, cos, tan, arcsin, arccos, arctan, lg and ln \n");
	printf("to calculate the result of a complex arithmetic expression like 'sin(1+2)', please input '(' and ')'\nIn other situations, there is no need to input '(' and ')' for example lg10, sin2\n\n");
	printf("When you use trigonometric functions, please input radians.\n\n");
	printf("OTHER NOTES:\nThe blank charater (such as a TAB or a SPACE) will be neglected.\n\n");
	printf("Let's start!\n\n\n");
}

int ConvertToPostfix(void)
{
    char *input, *PreCh = NULL;

    OperatorStackP = &OperatorStack;
    NewStack(OperatorStackP);
    PostfixNotation = NewQueue();
    printf("Please input the arithmetic expression:\n");

    while (TRUE) {
        input = GetBlock(sizeof(char)*STRINGSIZE);
        *input = getchar();
        if (*input == '\n') {             /*when there are no more tokens to read.*/
            char *temp;
            int i;
            
            if (PreCh == NULL) {
                printf("Input Nothing.\n");
                return ERROR;
            }
            *(input+1) = '\0';
            while (StackSize(OperatorStack)) {
            	temp = Pop(OperatorStackP);
                if ((*temp == '(') || (*temp == ')')) {
                    printf("Braces are not compatible.\n"); /*could be improve*/
                    return ERROR;
                }
                Enqueue(PostfixNotation, temp);
            }
                Enqueue(PostfixNotation, input);
				break;
        } else if (*input == ' ' || *input == '\t') {
        	FreeBlock(input); /*Throw away the blank character.*/
        } else if (*input >= '0' && *input <= '9') {    /*If the token is a number, then push it to the output queue.*/
            int i = 0;
            char *InputOrigin;
            
            InputOrigin = input;
            while (TRUE) {
                if (i == STRINGSIZE-1) {
                    printf("A number is too long.\n");
                    return ERROR;
                }
                *(++input) = getchar();
                if ((*input < '0' || *input > '9') && (*input != '.')) {
                    ungetc(*input, stdin);
                    break;
                }
                i++;
            }
            *input = '\0';
			Enqueue(PostfixNotation, InputOrigin);
            PreCh = input-1;
        } else if ((*input >= 'a' && *input <= 'z') || (*input >= 'A' && *input <= 'Z')) {   /*If the token is a function token, then push it onto the stack.*/
            if (*input == 'p' || *input == 'P') { /*There shouldn't be any function whose name starts with 'p' or 'P'*/
            	*(input+1) = getchar();
            	if (*(input+1) == 'i' || *(input+1) == 'I') {
            		*(input + 2) = getchar();
            		if (*(input + 2) >= 'a' && *(input + 2) <= 'z') {
                        printf("illegal function name or PI.\n");
                        return ERROR;
                    }
            		else {
            			ungetc(*(input+2), stdin);
            			input = "3.14159\0";
            			Enqueue(PostfixNotation, input);
            		}
            	}
                PreCh = input;
            } else if (*input == 'e' && ((*(input+1) = getchar()) < 'a' || *(input+1) > 'z')) {
            	ungetc(*(input+1), stdin);
            	input = "2.71828\0";
            	Enqueue(PostfixNotation, input);
                PreCh = input;
			} else {
            	int i = 1;
            	char *InputOrigin;
            
                if (*input == 'e') ungetc(*(input+1), stdin);
            	InputOrigin = input;
            	while (TRUE) {
                	if (i == STRINGSIZE-1) {
                        printf("The name of function is too long.\n"); 
                        return ERROR;
                    }
                	*(++input) = getchar();
                	i++;
                	if (*input < 'a' || *input > 'z') {
                    	ungetc(*input, stdin); 
                    	break;
                	}
            	}
            	*input = '\0';
                while (StackSize(OperatorStack)) {
                    char *temp;
				
                    temp = Top(OperatorStackP);
                    if ((*temp != '+') && (*temp != '-') && (*temp != '*') && (*temp != '/') && (*temp != '^') && (*temp != '!') && ((*temp < 'a') || (*temp > 'z'))) break;
                    if ((CheckAssociation(InputOrigin) && CompareAssociationPriority(*temp, *InputOrigin) >= 0) || (!CheckAssociation(InputOrigin) && CompareAssociationPriority(*temp, *InputOrigin) > 0)) Enqueue(PostfixNotation, Pop(OperatorStackP));
				    else break;
                }
            	Push(OperatorStackP, InputOrigin);
            	PreCh = input - 1;
            }	
        } else if ((*input == '+') || (*input == '-') || (*input == '*') || (*input == '/') || (*input == '^') || (*input == '!')) { /*If the token is an operator.*/
			if ((*input == '-') && (PreCh == NULL || *PreCh == '(')) {
                input = "-1\0";
                Enqueue(PostfixNotation, input);
                ungetc('*', stdin);
                PreCh = input;
            } else if ((*input == '+') && (PreCh == NULL || *PreCh == '(')) {
                FreeBlock(input);
            } else {
                *(input+1) = '\0';
                while (StackSize(OperatorStack)) {
                    char *temp;
				
                    temp = Top(OperatorStackP);
                    if ((*temp != '+') && (*temp != '-') && (*temp != '*') && (*temp != '/') && (*temp != '^') && (*temp != '!') && ((*temp < 'a') || (*temp > 'z'))) break;
                    if ((CheckAssociation(input) && CompareAssociationPriority(*temp, *input) >= 0) || (!CheckAssociation(input) && CompareAssociationPriority(*temp, *input) > 0)) Enqueue(PostfixNotation, Pop(OperatorStackP));  
				    else break;
                }
			    Push(OperatorStackP, input);
                PreCh = input;
            }
        } else if (*input == '(') {
        	*(input+1) = '\0';
            Push(OperatorStackP, input);
            PreCh = input;
        } else if (*input == ')') {
        	char *temp;
        	
            *(input+1) = '\0';
            while (TRUE) {
                if (StackSize(OperatorStack) == 0) {
                    printf("Braces are not compatible\n");
                    return ERROR;
                }
                temp = Pop(OperatorStackP);
                if (*temp == '(') break;
                Enqueue(PostfixNotation, temp);
            }
            if (StackSize(OperatorStack)) {
            	temp = Top(OperatorStackP);
            	if (*temp >= 'a' && *temp <= 'z') Enqueue(PostfixNotation, Pop(OperatorStackP));
            }
            PreCh = input;
        } else if (*input == ',') {
            *(input+1) = '\0';
            string temp;

            while (StackSize(OperatorStack)) {
                if (*(temp = Top(OperatorStackP)) == '(') break;
                Enqueue(PostfixNotation, Pop(OperatorStackP));
            }
            if (*(temp = Top(OperatorStackP)) != '(') {
                printf("Either the separator was misplaced or parentheses were mismatched.\n");
                return ERROR;
            }
            PreCh = input;
        }
          else {
            printf("there is a illegal character.\n");
            return ERROR;
        }
    }
    return SUCCESS;
}

bool CheckAssociation(string op)
{
     if (StringEqual(op, "+") || StringEqual(op, "-") || StringEqual(op, "*") || StringEqual(op, "/") || StringEqual(op, "^")) return 1;
     else return 0;
}

int CompareAssociationPriority(char op1, char op2)
{
    int priority1, priority2;

    if (op1 == '^' || op1 == '!' || (op1 >= 'a' && op1 <= 'z')) priority1 = 3;
    else if (op1 == '*' || op1 == '/') priority1 = 2;
    else if (op1 == '+' || op1 == '-') priority1 = 1;
    if (op2 == '^'|| op1 == '!' || (op2 >= 'a' && op2 <= 'z')) priority2 = 3;
    else if (op2 == '*' || op2 == '/') priority2 = 2;
    else if (op2 == '+' || op2 == '-') priority2 = 1;

    if (priority1 > priority2) return 1;
    else if (priority1 == priority2) return 0;
    else return -1;
}

void Process(void)
{
    int i = 0;
    char *str;
    double TempOperand[MAXSIZE];
    double result;

	OperandStackP = &OperandStack;
    NewStack(OperandStackP);

    while ((*(str = Dequeue(PostfixNotation))) != '\n') {
        if (*str >= '0' && *str <= '9') {
        	Push(OperandStackP, str);
        } 
        else if (*str == '-' && *(str+1) >= '0' && *(str+1) <= '9') Push(OperandStackP, str); 
        else Push(OperandStackP, RealToString(CallFunction(str, OperandStackP)));
    }
    if (StackSize(OperandStack) == 1) Output(Pop(OperandStackP));
    else printf("Too much operands.\n");
}

double CallFunction(string NameOfFunction, struct stack_node **OperandStackP)
{
    double op1, op2;
	
    if (StringEqual(NameOfFunction, "sin")) return sin(strtod(Pop(OperandStackP), NULL)); /*strtod() is a function to convert string to double.*/
    else if (StringEqual(NameOfFunction, "cos")) return cos(strtod(Pop(OperandStackP), NULL));
    else if (StringEqual(NameOfFunction, "tan")) return tan(strtod(Pop(OperandStackP), NULL));
    else if (StringEqual(NameOfFunction, "arcsin")) return asin(strtod(Pop(OperandStackP), NULL));
    else if (StringEqual(NameOfFunction, "arccos")) return acos(strtod(Pop(OperandStackP), NULL));
    else if (StringEqual(NameOfFunction, "arctan")) return atan(strtod(Pop(OperandStackP), NULL));
    else if (StringEqual(NameOfFunction, "sqrt")) return SQRT(strtod(Pop(OperandStackP), NULL));
    else if (StringEqual(NameOfFunction, "abs")) return Abs(strtod(Pop(OperandStackP), NULL));
    else if (StringEqual(NameOfFunction, "exp")) return exp(strtod(Pop(OperandStackP), NULL));
    else if (StringEqual(NameOfFunction, "ln")) return LN(strtod(Pop(OperandStackP), NULL));
    else if (StringEqual(NameOfFunction, "lg")) return LG(strtod(Pop(OperandStackP), NULL));
    else if (StringEqual(NameOfFunction, "!")) return factorial(strtod(Pop(OperandStackP), NULL));
    else if (StringEqual(NameOfFunction, "^")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        op2 = strtod(Pop(OperandStackP), NULL);
        return pow(op2, op1);
    } else if (StringEqual(NameOfFunction, "*")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        op2 = strtod(Pop(OperandStackP), NULL);
        return op1 * op2;
    } else if (StringEqual(NameOfFunction, "/")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        if (op1 == 0) {
            printf("The denominator of the numerator cannot have the value zero.\n");
            longjmp(JumpBuffer, 1);
        }
        op2 = strtod(Pop(OperandStackP), NULL);
        return op2 / op1;
    } else if (StringEqual(NameOfFunction, "+")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        op2 = strtod(Pop(OperandStackP), NULL);
        return op1 + op2;
    } else if (StringEqual(NameOfFunction, "-")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        op2 = strtod(Pop(OperandStackP), NULL);
        return op2 - op1;
    } else if (StringEqual(NameOfFunction, "log")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        op2 = strtod(Pop(OperandStackP), NULL);
        return LOG(op2, op1);
    } else {
        printf("illegal function name.\n");
        longjmp(JumpBuffer, 1);
    }
}

void Output(string str)
{
    printf("The answer is %.2f\n\n", strtod(str, NULL));
    FreeBlock(OperandStack);
    FreeBlock(OperandStackP);
    FreeBlock(PostfixNotation);
}
