#include "stdio.h"
#include "math.h"
#include "strlib.h" 

double factorial(double n)
{
    int i;
    double result = 1;

    if ((n - (int) n) || n < 0) Error("The function factorial needs a positive integer.\n"); /*check whether n is a integer.*/
    if (n == 0) return 1;
    for (i = 1; i <= n; i++) result *= i;
    return result;
}

double Abs(double x)
{
	if (x >= 0) return x;
	else return -x;
}

double LOG(double base, double antilog)
{
    if (base < 0 || antilog < 0) Error("The base and antilog of logarithmic function should be a positive number.\n");
    return log(antilog)/log(base);
}

double SQRT(double x)
{
    if (x < 0) Error("The funcion sqrt() needs a positive number.\n");
    return sqrt(x);
}

double LG(double antilog)
{
    if (antilog < 0) Error("The antilog of logarithmic function should be a positive number.\n");
    return log10(antilog);
}

double LN(double antilog)
{
    if (antilog < 0) Error("The antilog of logarithmic function should be a positive number.\n");
    return log(antilog);
}