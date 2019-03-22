//------------------------------------------
// DISPLAY API (Application Programming Interface)
// Also known as DISPLAY HAL (Hardware Abstraction Layer)
// HAL is a specific form of API that designs the interface with a certain hardware

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <ti/grlib/grlib.h>


#define MY_BLACK GRAPHICS_COLOR_BLACK
#define MY_WHITE GRAPHICS_COLOR_WHITE


void InitGraphics();
void LCDClearDisplay(int color);
void LCDDrawChar(unsigned row, unsigned col, int8_t c);
void PrintString(char *str, int row, int col);

//***********MY FUNCTIONS ADDED BELOW HERE**************************
void DrawTitleScreen(); //keep in mind that it is already drawn in LAB3_main.c


void DrawStartScreen(); //keep in mind that it is already drawn in LAB3_main.c

/**
 * DrawInstructionsScreen(): Draws the outline of the instructions screen.
 * Explains how to play the game
 * BONUS: Add scroll functionality? Seems too hard :/
 *
 */
void DrawInstructionScreen();
void DrawScoreScreen(unsigned int userScore, bool on_ff);
void DrawGameScreen(unsigned int userScrore, unsigned int highScore);
void DrawPauseScreen();
void DrawGameOverScreen(int userScore);
void moveGameMenuArrow(int *yPointer);
void moveGamePauseArrow(int *yPointer);
void moveCar();
void moveTree00(int code);
void moveTree01(int code);
void moveTree02(int code);
bool getTree00Status();
bool getTree01Status();
bool getTree02Status();
void setTree00Status(bool status);
void setTree01Status(bool status);
void setTree02Status(bool status);
int addScore();
int getTree00YMin();
int getTree00YMax();
int getTree01YMin();
int getTree01YMax();
int getTree02YMin();
int getTree02YMax();
void setForegroundToBlack();
void setForegroundToWhite();
const char* intToASCII(unsigned score);
bool crashTree00(int code);
bool crashTree01(int code);
bool crashTree02(int code);
void DrawHiScoreGameBoard();
void DrawUserScore(unsigned int curScore, unsigned int prevScore);
bool reset(bool reset);
void pausedCar();
int getHighScore(int userScore);
int randomInt();


#endif /* DISPLAY_H_ */
