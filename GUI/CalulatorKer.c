#include "CalulatorKer.h"

void ker(char STR[])
{
	int i;
	//char STR[1000];//
    while (TRUE){
    	setjmp(JumpBuffer);
        //fflush(stdin);
    	//strcpy(array, "Please input the arithmetic expression:\n");
		//printf("%s", array);
		i = 0;
    	while((STR[i] = getchar()) != '\n') i++;//
    	STR[i] = '\0';
        if (ConvertToPostfix(STR)) Process();
    }
    getchar(); 
}

int ConvertToPostfix(char STR[])//
{
    char *input, *PreCh = NULL;
 	int n = 0;
    OperatorStackP = &OperatorStack;
    NewStack(OperatorStackP);
    PostfixNotation = NewQueue();
    
    while (TRUE) {                          
        input = GetBlock(sizeof(char)*STRINGSIZE);
        *input = STR[n];//
        if (*input == '\0') {             /*when there are no more tokens to read.*/
            char *temp;
            int i;
            
            if (PreCh == NULL) {
            	strcpy(array, "Input Nothing.\n");
				//printf("%s", array);
            	return ERROR;
            }
            *(input+1) = '\0';
            while (StackSize(OperatorStack)) {
            	temp = Pop(OperatorStackP);
                if ((*temp == '(') || (*temp == ')')) {
                	strcpy(array, "Braces are not compatible.\n");
					//printf("%s", array);
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
                	strcpy(array, "A number is too long.\n");
					//printf("%s", array);
                    return ERROR;
                }
                *(++input) = STR[n + i + 1];//
                if ((*input < '0' || *input > '9') && (*input != '.')) {
                    break;
                }
                i++;
            }
            n = n + i ;
            *input = '\0';
			Enqueue(PostfixNotation, InputOrigin);
            PreCh = input-1;
        } else if ((*input >= 'a' && *input <= 'z') || (*input >= 'A' && *input <= 'Z')) {   /*If the token is a function token, then push it onto the stack.*/
            if (*input == 'p' || *input == 'P') { /*There shouldn't be any function whose name starts with 'p' or 'P'*/
            	*(input+1) = STR[n + 1];//
            	if (*(input+1) == 'i' || *(input+1) == 'I') {
            		*(input + 2) = STR[n + 2];//
            		if (*(input + 2) >= 'a' && *(input + 2) <= 'z') {
            			strcpy(array, "illegal function name or PI.\n");
						//printf("%s", array);
                        return ERROR;
                    }
            		else {
            			input = "3.14159\0";
            			Enqueue(PostfixNotation, input);
            		}
            		n = n + 1;
            	}
                PreCh = input;
            } else if (*input == 'e' && ((*(input+1) = STR[n + 1]) < 'a' || *(input+1) > 'z')) {
            	input = "2.71828\0";
            	Enqueue(PostfixNotation, input);
                PreCh = input;
			} else {
            	int i = 1;
            	char *InputOrigin;
            
             	InputOrigin = input;
            	while (TRUE) {
                	if (i == STRINGSIZE-1) {
                 		strcpy(array, "The name of function is too long.\n");
						//printf("%s", array);
                        return ERROR;
                    }
                	*(++input) = STR[n + i];//
                	i++;
                	if (*input < 'a' || *input > 'z') {
                    	break;
                	}
            	}
            	n = n + i - 2;
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
                input = "-1\0";//
                Enqueue(PostfixNotation, input);
                *(STR + n)='*';//
				n = n - 1;  //             
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
                	strcpy(array, "Braces are not compatible\n");
					//printf("%s", array);
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
            	strcpy(array, "Either the separator was misplaced or parentheses were mismatched.\n");
				//printf("%s", array);
                return ERROR;
            }
            PreCh = input;
        } else {
          	strcpy(array, "there is a illegal character.\n");
		  	//printf("%s", array);
           	return ERROR;
       	  }
        n ++;
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

    while ((*(str = Dequeue(PostfixNotation))) != '\0') {
        if (*str >= '0' && *str <= '9') {
        	Push(OperandStackP, str);
        } 
        else if (*str == '-' && *(str+1) >= '0' && *(str+1) <= '9') Push(OperandStackP, str); 
        else Push(OperandStackP, RealToString(CallFunction(str, OperandStackP)));
    }
    if (StackSize(OperandStack) == 1) Output(Pop(OperandStackP));
    else{
    	strcpy(array, "Too much operands.\n");
		//printf("%s", array);
	} 
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
        	strcpy(array, "The denominator of the numerator cannot have the value zero.\n");
			//printf("%s", array);
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
    	strcpy(array, "illegal function name.\n");
		//printf("%s", array);
	    longjmp(JumpBuffer, 1);
    }
}

void Output(string str)
{
	sprintf(array, "The answer is %.2f\n\n", strtod(str, NULL));
	//printf("%s", array);
    FreeBlock(OperandStack);
    FreeBlock(OperandStackP);
    FreeBlock(PostfixNotation);
}
