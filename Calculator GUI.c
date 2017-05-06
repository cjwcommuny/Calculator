#include <stdio.h>
#include "math.h"
#include "genlib.h"
#include "simpio.h"
#include "queue.h"
#include "stack.h" 
#include <stdlib.h>
#include "mathematic.h"
#include <setjmp.h>
#include "graphics.h"
#include "extgraph.h"

//#define BUTTON_SHAPE() 
#define BUTTON_COLUMN 5
#define BUTTON_ROW 9
#define MOVE_COLOR Red
#define SELECT_COLOR Black
#define CHECK_WINDOW_SIZE 1
#define CHECK_WINDOW_SIZE_TIME 200
#define ERROR_LIMIT 0.02

struct Point {
    double x;
    double y;
};

struct button {
    //char *text;
    char *move_color;
    char *click_color;
};
struct window_size {
    double width;
    double height;
};

struct calculator_frame {
    //struct button *button_frame[BUTTON_ROW][BUTTON_COLUMN];
    char *button_text[BUTTON_ROW][BUTTON_COLUMN];
    struct Point output_frame[6];
};
struct Point *CurrentPoint;
struct Point *PreviousPoint;
struct window_size *WindowSize;
struct calculator_frame *calculatorP;
struct calculator_frame calculatorTemp = {
        "MC", "MR", "M+", "M-", "MS",
        "CE", "C", "BackSpace", "", "",
        "sin", "cos", "tan", "abs", "!",
        "arcsin", "arccos", "arctan", "exp", "log",
        "sqrt", "^", "lg", "ln", "/",
        "Pi", "7", "8", "9", "*",
        "e", "4", "5", "6", "-",
        "-", "1", "2", "3", "+",
        "(", ")", "0", ".", "="
    };
//struct button *button_collection[BUTTON_ROW][BUTTON_COLUMN];
double OutputHeight;
double ButtonHeight, ButtonWidth;
int testcount = 0;
/*char *ButtonText[] = {
        "MC", "MR", "M+", "M-", "MS",
        "CE", "C", "", "",
        "sin", "cos", "tan", "abs", "!",
        "arcsin", "arccos", "arctan", "exp", "log",
        "sqrt", "^", "lg", "ln", "/",
        "Pi", "7", "8", "9", "*",
        "e", "4", "5", "6", "-",
        "-", "1", "2", "3", "+",
        "(", ")", "0", ".", "="
    };*/

void CharEventProcess(char c);
void MouseEventProcess(int x, int y, int button, int event);
void TimerEventProcess(int timerID);

void DrawCalculatorFrame(void);
void InitCalculator(void);
void RefreshDisplay(void);
void DrawOutputFrame(void);
void DrawRectangle(double x, double y, double width, double height);
void PrintText(void);
void PrintTextCenter(char *text, struct Point);


void Main()
{
    SetWindowTitle("Calculaor");
    InitGraphics();
    InitConsole();

    CurrentPoint = GetBlock(sizeof(struct Point));
    PreviousPoint = GetBlock(sizeof(struct Point));
    PreviousPoint->x = 0;
    PreviousPoint->y = 0;
    CurrentPoint->x = 0;
    CurrentPoint->y = 0;

    InitCalculator();
    DrawCalculatorFrame();
    registerCharEvent(CharEventProcess);
	registerMouseEvent(MouseEventProcess);
	//registerTimerEvent(TimerEventProcess);

    //WindowSize = GetBlock(sizeof(struct window_size));
    //startTimer(CHECK_WINDOW_SIZE, CHECK_WINDOW_SIZE_TIME);
}

void RefreshDisplay(void)
{
    SetEraseMode(TRUE);
    StartFilledRegion(1);
    DrawRectangle(0, 0, GetWindowWidth(), GetWindowHeight());
    EndFilledRegion();
    //MovePen(CurrentPoint->x, CurrentPoint->y);
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

    calculatorP = &calculatorTemp;
}

void DrawCalculatorFrame(void)
{
    double window_width, window_height;
    double gap_width, gap_height;
    //double ButtonWidth, ButtonHeight;

    //printf("TEST:here\n");
    window_width = GetWindowWidth();
    window_height = GetWindowHeight();
    gap_width = window_width / 15;
    gap_height = gap_width;
    ButtonWidth = window_width / 5;
    OutputHeight = ButtonHeight = (window_height - 2 * gap_height) / 11;
    //OutputWidth = window_width / 5;
    //printf("TEST:%f\n", GetWindowWidth());
    
    calculatorP->output_frame[0].x = gap_width;
    calculatorP->output_frame[0].y = window_height - gap_height;
    calculatorP->output_frame[1].x = window_width - gap_width;
    calculatorP->output_frame[1].y = window_height - gap_height;
    calculatorP->output_frame[2].x = window_width - gap_width;
    calculatorP->output_frame[2].y = window_height - gap_height - 2 * OutputHeight;
    calculatorP->output_frame[3].x = gap_width;
    calculatorP->output_frame[3].y = window_height - gap_height - 2 * OutputHeight;
    calculatorP->output_frame[4].x = gap_width;
    calculatorP->output_frame[4].y = window_height - gap_height - OutputHeight;
    calculatorP->output_frame[5].x = window_width - gap_width;
    calculatorP->output_frame[5].y = window_height - gap_height - OutputHeight;
    //printf("test");
    //printf("TEST:%f\n", calculatorP->output_frame[0].x);
    DrawOutputFrame();

    PrintText();
}

void DrawOutputFrame(void)
{
    //printf("TEST:here\n");
    //printf("TEST:%f, %f\n", calculatorP->output_frame[0].x, calculatorP->output_frame[0].y);
    //printf("TEST:%f, %f\n", calculatorP->output_frame[3].x, calculatorP->output_frame[3].y);
    MovePen(calculatorP->output_frame[0].x, calculatorP->output_frame[0].y);
    DrawLine(calculatorP->output_frame[1].x - calculatorP->output_frame[0].x, 
             calculatorP->output_frame[1].y - calculatorP->output_frame[0].y);
    DrawLine(calculatorP->output_frame[2].x - calculatorP->output_frame[1].x, 
             calculatorP->output_frame[2].y - calculatorP->output_frame[1].y);
    DrawLine(calculatorP->output_frame[3].x - calculatorP->output_frame[2].x, 
             calculatorP->output_frame[3].y - calculatorP->output_frame[2].y);
    DrawLine(calculatorP->output_frame[0].x - calculatorP->output_frame[3].x, 
             calculatorP->output_frame[0].y - calculatorP->output_frame[3].y);
    MovePen(calculatorP->output_frame[4].x, calculatorP->output_frame[4].y);
    DrawLine(calculatorP->output_frame[5].x - calculatorP->output_frame[4].x, 
             calculatorP->output_frame[5].y - calculatorP->output_frame[4].y);

}

void PrintText(void)
{
    int i, j;
    struct Point TextPosition;

    //printf("TEST:here\n");
    for (i = 0; i < BUTTON_ROW; i++) {
        for (j = 0; j < BUTTON_COLUMN; j++) {
            TextPosition.x = (j+0.5)*ButtonWidth;
            TextPosition.y = (BUTTON_ROW-i)*ButtonHeight;
            //testcount++;
            //printf("test:%d\n", testcount);
            //printf("TEST:(%d, %d):%f, %f\n", i , j, TextPosition.x, TextPosition.y);
            //printf("test\n");
            //if (i == 0 && j == 0) printf("TEST:%f, %f\n", ButtonWidth, TextPosition.x);
            //printf("%f\n", ButtonWidth);
            PrintTextCenter(calculatorP->button_text[i][j], TextPosition);
        }
    }
}

void PrintTextCenter(char *text, struct Point point)
{
    double StrWidth = TextStringWidth(text);

    //printf("TEST:#%s#\n", text);
    //testcount++;
    MovePen(point.x - StrWidth/2, point.y - GetFontHeight()/2);
    DrawTextString(text);
    //printf("test:%d\n", testcount);
}

void MouseEventProcess(int x, int y, int button, int event)
{
    PreviousPoint->x = CurrentPoint->x;
    PreviousPoint->y = CurrentPoint->y;
    CurrentPoint->x = ScaleXInches(x);
    CurrentPoint->y = GetWindowHeight() - ScaleXInches(y);

    switch (event) {
        case BUTTON_DOWN:
            switch (button) {
                case LEFT_BUTTON:
                    
                    break;
            }
            break;
        case BUTTON_UP:
            break;
    }
}

void KeyboardEventProcess(int key,int event)
{

}

void CharEventProcess(char c)
{

}

void TimerEventProcess(int timerID)
{
    switch (timerID) {
        case CHECK_WINDOW_SIZE: {
            double x, y;

            x = GetWindowWidth();
            y = GetWindowHeight();
            printf("TEST:%f, %f\n", x, y);
            if (fabs(x - WindowSize->width) > ERROR_LIMIT || fabs(y - WindowSize->height) > ERROR_LIMIT) {
                //printf("test\n");
                DrawCalculatorFrame();
            }
            WindowSize->width = x;
            WindowSize->height = y;
            break;
        }
    }
}

void DrawRectangle(double x, double y, double width, double height)
{
    MovePen(x, y);
    DrawLine(width, 0);
    DrawLine(0, height);
    DrawLine(-width, 0);
    DrawLine(0, -height);
}
