#include "stdio.h"
#include "math.h"
#include "genlib.h"
#include "simpio.h"
#include "queue.h"
#include "stack.h" 
#include "stdlib.h"

#define MAXSIZE 100
#define STRINGSIZE 10

/*解决两个bug，使程序支持类似lg10的语句*/
void Prompt(void);
void ConvertToPostfix(void);
void Process(void);
bool CheckAssociation(string op);
int CompareAssociationPriority(char op1, char op2);
int NumOperand(string OperatorNameStr);
double CallFunction(string NameOfFunction, struct stack_node **OperandStackP);
void Output(string str);
double factorial(int n);
double Abs(double x);
double LOG(double base, double antilog);

queueADT PostfixNotation;
struct stack_node **OperatorStackP;
struct stack_node *OperatorStack;
struct stack_node **OperandStackP;
struct stack_node *OperandStack;

main()
{
    /*Prompt();*/
    while (TRUE){
        ConvertToPostfix();
        Process();
    }
}

void ConvertToPostfix(void)
{
    char *input, *PreCh = NULL;

    OperatorStackP = &OperatorStack;
    NewStack(OperatorStackP);
    PostfixNotation = NewQueue();
    printf("Please input the arithmetic expression:\n");

    while (TRUE) {                          
        input = GetBlock(sizeof(char)*STRINGSIZE);
        *input = getchar();
        if (*input == '\n') {               /*when there are no more tokens to read.*/
            char *temp;
            int i;
            
            *(input+1) = '\0';
            while (StackSize(OperatorStack)) {
            	temp = Pop(OperatorStackP);
                if ((*temp == '(') || (*temp == ')')) Error("Braces are not compatible\n"); /*could be improve*/
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
                if (i == STRINGSIZE-1) Error("A number is too long.");
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
        } else if (*input >= 'a' && *input <= 'z') {   /*If the token is a function token, then push it onto the stack.*/
            if (*input == 'p' || *input == 'P') { /*There shouldn't be any function whose name starts with 'p' or 'P'*/
            	*(input+1) = getchar();
            	if (*(input+1) == 'i' || *(input+1) == 'I') {
            		*(input + 2) = getchar();
            		if (*(input + 2) >= 'a' && *(input + 2) <= 'z') Error("illeagal function name or PI.\n"); /*?????*/
            		else {
            			ungetc(*(input+2), stdin);
            			input = "3.14159\0";
            			Enqueue(PostfixNotation, input);
            		}
            	}
            } else if (*input == 'e' && ((*(input+1) = getchar()) < 'a' || *(input+1) > 'z')) {
            	ungetc(*(input+1), stdin);
            	input = "2.71281\0";
            	Enqueue(PostfixNotation, input);
			} else {
            	int i = 1;
            	char *InputOrigin;
            
            	InputOrigin = input;
            	while (TRUE) {
                	if (i == STRINGSIZE-1) Error("The name of function is too long."); /*there may be a bug.*/
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
                    if ((*temp != '+') && (*temp != '-') && (*temp != '*') && (*temp != '/') && (*temp != '^') && (*temp != '!') && (*temp < 'a') && (*temp > 'z')) break;
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
            } else {
                *(input+1) = '\0';
                while (StackSize(OperatorStack)) {
                    char *temp;
				
                    temp = Top(OperatorStackP);
                    if ((*temp != '+') && (*temp != '-') && (*temp != '*') && (*temp != '/') && (*temp != '^') && (*temp != '!') && (*temp < 'a') && (*temp > 'z')) break;
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
                if (StackSize(OperatorStack) == 0) Error("Braces are not compatible\n");
                temp = Pop(OperatorStackP);
                if (*temp == '(') break;
                Enqueue(PostfixNotation, temp);
            }
            if (StackSize(OperatorStack)) {
            	temp = Top(OperatorStackP);
            	if (*temp >= 'a' && *temp <= 'z') Enqueue(PostfixNotation, Pop(OperatorStackP));
            }
            PreCh = input;
        } else if (*input == ',') FreeBlock(input);
          else {
            Error("there is a illegal character.");
        }
    }
    FreeBlock(OperatorStack);
    FreeBlock(OperatorStackP);
}

bool CheckAssociation(string op)
{
     if (StringEqual(op, "+") || StringEqual(op, "-") || StringEqual(op, "*") || StringEqual(op, "/") || StringEqual(op, "^")) return 1;
     else return 0;
}

int CompareAssociationPriority(char op1, char op2)
{
    int priority1, priority2;

    if (op1 == '^' || op1 == '!' || (op1 >= 'a' && op2 <= 'z')) priority1 = 3;
    else if (op1 == '*' || op1 == '/') priority1 = 2;
    else if (op1 == '+' || op1 == '-') priority1 = 1;
    if (op2 == '^'|| op1 == '!' || (op1 >= 'a' && op2 <= 'z')) priority2 = 3;
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
        else if (*str == '-' && *(str+1) >= '0' && *(str+1) <= '9') Push(OperandStackP, str); /*????*/
        else Push(OperandStackP, RealToString(CallFunction(str, OperandStackP)));
    }
    if (StackSize(OperandStack) == 1) Output(Pop(OperandStackP));
    else Error("Too much operands.");
}

double CallFunction(string NameOfFunction, struct stack_node **OperandStackP)
{
    double op1, op2;

    if (StringEqual(NameOfFunction, "sin")) return sin(strtod(Pop(OperandStackP), NULL)); /*strtod() is a function to convert string to double.*/
    if (StringEqual(NameOfFunction, "cos")) return cos(strtod(Pop(OperandStackP), NULL));
    if (StringEqual(NameOfFunction, "tan")) return tan(strtod(Pop(OperandStackP), NULL));
    if (StringEqual(NameOfFunction, "arcsin")) return asin(strtod(Pop(OperandStackP), NULL));
    if (StringEqual(NameOfFunction, "arccos")) return acos(strtod(Pop(OperandStackP), NULL));
    if (StringEqual(NameOfFunction, "arctan")) return atan(strtod(Pop(OperandStackP), NULL));
    if (StringEqual(NameOfFunction, "sqrt")) return sqrt(strtod(Pop(OperandStackP), NULL));
    if (StringEqual(NameOfFunction, "abs")) return Abs(strtod(Pop(OperandStackP), NULL));
    if (StringEqual(NameOfFunction, "exp")) return exp(strtod(Pop(OperandStackP), NULL));
    if (StringEqual(NameOfFunction, "ln")) return log(strtod(Pop(OperandStackP), NULL));
    if (StringEqual(NameOfFunction, "lg")) return log10(strtod(Pop(OperandStackP), NULL));
    if (StringEqual(NameOfFunction, "!")) return factorial((int) strtod(Pop(OperandStackP), NULL));

    if (StringEqual(NameOfFunction, "^")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        op2 = strtod(Pop(OperandStackP), NULL);
        return pow(op2, op1);
    }
    if (StringEqual(NameOfFunction, "*")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        op2 = strtod(Pop(OperandStackP), NULL);
        return op1 * op2;
    }
    if (StringEqual(NameOfFunction, "/")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        op2 = strtod(Pop(OperandStackP), NULL);
        return op2 / op1;
    }
    if (StringEqual(NameOfFunction, "+")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        op2 = strtod(Pop(OperandStackP), NULL);
        return op1 + op2;
    }
    if (StringEqual(NameOfFunction, "-")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        op2 = strtod(Pop(OperandStackP), NULL);
        /**/printf("+: %f#%f\n", op1, op2);
        return op2 - op1;
    }
    if (StringEqual(NameOfFunction, "log")) {
        op1 = strtod(Pop(OperandStackP), NULL);
        op2 = strtod(Pop(OperandStackP), NULL);
        return LOG(op2, op1);
    }
}

void Output(string str)
{
    printf("The answer is %.2f\n\n", strtod(str, NULL));
    FreeBlock(OperandStack);
    FreeBlock(OperandStackP);
    FreeBlock(PostfixNotation);
}

double factorial(int n)
{
    int i;
    double result = 1;

    for (i = 1; i <= n; i++) result *= i;;
    return result;
}

double Abs(double x)
{
	if (x >= 0) return x;
	else return -x;
}

double LOG(double base, double antilog)
{
    return log(antilog)/log(base);
}