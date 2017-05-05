#include <stdio.h>
#include "math.h"
#include "genlib.h"
#include "simpio.h"
#include "queue.h"
#include "stack.h" 
#include <stdlib.h>
#include "mathematic.h"
#include <setjmp.h>

//#define BUTTON_SHAPE() 
#define BUTTON_COLUMN 5
#define BUTTON_ROW 9
#define MOVE_COLOR Red
#define SELECT_COLOR Black

struct Point {
    double x;
    double y;
};

struct button {
    //char *text;
    char *move_color;
    char *click_color;
};

struct calculator_frame {
    //struct button *button_frame[BUTTON_ROW][BUTTON_COLUMN];
    Point output_frame[6];
    char *button_text[][] = {
        {"MC", "MR", "M+", "M-", "MS"},
        {"CE", "C", "", ""},
        {"sin", "cos", "tan", "abs", "!"},
        {"arcsin", "arccos", "arctan", "exp", "log"},
        {"sqrt", "^", "lg", "ln", "÷"},
        {"Pi", "7", "8", "9", "×"},
        {"e", "4", "5", "6", "-"},
        {"-", "1", "2", "3", "+"},
        {"(", ")", "0", ".", "="}
    };
};

struct calculator_frame *calculatorP;
//struct button *button_collection[BUTTON_ROW][BUTTON_COLUMN];
double OutputHeight;
double ButtonHeight, ButtonWidth;

void CharEventProcess(char c);
void MouseEventProcess(int x, int y, int button, int event);
void TimerEventProcess(int timerID);

void DrawCalculatorFrame(void);
void InitCalculator(void);
void RefreshDisplay(void);

void Main()
{
    SetWindowTitle("Calculaor");
    InitGraphics();
    InitCalculator();
    DrawCalculatorFrame();
    registerCharEvent(CharEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerTimerEvent(TimerEventProcess);
}

void RefreshDisplay(void)
{
    SetEraseMode(TRUE);
    StartFilledRegion(1);
    DrawRectangle(0, 0, GetWindowWidth(), GetWindowHeight());
    EndFilledRegion();
    MovePen(CurrentPoint->x, CurrentPoint->y);
    SetEraseMode(FALSE);
}

void InitCalculator(void)
{
    //int i, j;
    //struct button *buttonP;
    //double WindowWidth, WindowHeight;
    //double ButtonWidth, ButtonHeight;
    //WindowWidth = GetWindowWidth();
    //WindowHeight = GetWindowHeight();
    //int i;

    calculatorP = GetBlock(sizeof(struct calculator_frame));
}

void DrawCalculatorFrame(void);
{
    double window_width, window_height;
    double gap_width, gap_height;
    //double ButtonWidth, ButtonHeight;

    window_width = GetWindowWidth();
    window_height = GetWindowHeight();
    gap_width = window_width / 15;
    gap_height = gap_width;

    //将算法与数据分离,把输出框坐标存入,然后画图
}

void KeyboardEventProcess(int key,int event)
{

}

void CharEventProcess(char c)
{

}

void TimerEventProcess(int timerID)
{

}