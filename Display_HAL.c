#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ADC_HAL.h>
//Opening Screen Drawn in LAB3_main.c
//Game Menu Screen Drawn in LAB3_main.c

Graphics_Context g_sContext; //global varible 2.
bool glbl_treeStatus[3] = {false, false, false}; //the tree00 is initially 0
bool glbl_reset = false;
int glbl_carXMin = 8;
int glbl_treeYMin[3];

void InitGraphics() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
    Graphics_initContext(&g_sContext,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontCmtt16);
    Graphics_clearDisplay(&g_sContext);
}

void LCDClearDisplay(int color) {
    Graphics_setBackgroundColor(&g_sContext, color);
    Graphics_clearDisplay(&g_sContext);
}


void LCDDrawChar(unsigned row, unsigned col, int8_t c) {
    Graphics_drawString(&g_sContext,
                        &c,
                        1,
                        8 * (col % 16),
                        16 * (row % 8),
                        OPAQUE_TEXT);
}

void PrintString(char *str, int row, int col) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        LCDDrawChar(row,  col, str[i]);
        col++;
        if (col >= 16) {
            col = 0;
            row++;
            if (row >= 8) {
                row = 0;
            }
        }
    }
}

//***********MY FUNCTIONS ADDED BELOW HERE**************************

int randomInt(){
    return (rand()%3);
}




void moveGameMenuArrow(int *yPointer) {

    static enum {REST, UP, DOWN, IDLE, ACTION} glbl_menuState = REST;
    unsigned static vx, vy;
    getSampleJoyStick(&vx, &vy);
    int glbl_yJoyStick = vy;

    switch(glbl_menuState)
    {
        case REST:
            if(glbl_yJoyStick >= 13000)
            {
                    glbl_menuState = UP;
                    //moves arrow up
                    PrintString("  ", *yPointer, 0);
                    (*yPointer)--;
                    if(*yPointer <= 1)
                        *yPointer = 4;
                    PrintString("->", *yPointer, 0);
            }
            else if(glbl_yJoyStick <= 3000)
            {
                glbl_menuState = DOWN;
                //move arrow down
                PrintString("  ", *yPointer, 0);
                (*yPointer)++;
                if (*yPointer >= 5)
                    *yPointer = 2;
                PrintString("->", *yPointer, 0);
            }
        break;
        case UP:
            if(glbl_yJoyStick >= 6000 && glbl_yJoyStick <= 10000)
                glbl_menuState = REST;
        break;
        case DOWN:
            if(glbl_yJoyStick >= 6000 && glbl_yJoyStick <= 10000)
                glbl_menuState = REST;
            //printf("DOWN State|yValue: %d\n", glbl_yJoyStick);
        break;
        case IDLE:

        break;
    }
}


void moveGamePauseArrow(int *yPointer) {
    static enum {REST, UP, DOWN, IDLE, ACTION} glbl_menuState = REST;
    unsigned static vx, vy;
    getSampleJoyStick(&vx, &vy);
    int glbl_yJoyStick = vy;
    switch(glbl_menuState)
    {
        case REST:
            if(glbl_yJoyStick >= 13000)
            {
                    glbl_menuState = UP;
                    //moves arrow up
                    PrintString("  ", *yPointer, 0);
                    (*yPointer)--;
                    if(*yPointer <= 1)
                        *yPointer = 3;
                    PrintString("->", *yPointer, 0);
            }
            else if(glbl_yJoyStick <= 3000)
            {
                glbl_menuState = DOWN;
                //move arrow down
                PrintString("  ", *yPointer, 0);
                (*yPointer)++;
                if (*yPointer >= 4)
                    *yPointer = 2;
                PrintString("->", *yPointer, 0);
            }
        break;
        case UP:
            if(glbl_yJoyStick >= 6000 && glbl_yJoyStick <= 10000)
                glbl_menuState = REST;
        break;
        case DOWN:
            if(glbl_yJoyStick >= 6000 && glbl_yJoyStick <= 10000)
                glbl_menuState = REST;
            //printf("DOWN State|yValue: %d\n", glbl_yJoyStick);
        break;
        case IDLE:

        break;
    }
}

void pausedCar(){
    extern tImage  myredcar008BPP_UNCOMP; //Defined in myredcar00.c file, I don't think static is necessary
    Graphics_drawImage(&g_sContext, &myredcar008BPP_UNCOMP, glbl_carXMin, 112);
}

void moveCar()
{
    static enum  {REST, LEFT, RIGHT} carState = REST;
    extern tImage  myredcar008BPP_UNCOMP; //Defined in myredcar00.c file, I don't think static is necessary
    unsigned static vx, vy;
    static int i = 8;
    static bug = true;
    static int xImage = 8; //initial starting point, top right x position of 16x16 box
    static Graphics_Rectangle rect = {8, 112, 23, 127};

    if(glbl_reset)
    {
        i = 8;
        rect.xMin = i;
        rect.yMin = 112;
        rect.yMax = 127;
        rect.xMax = 23;
        xImage = 8;
        glbl_carXMin = rect.xMin;
    }
    else
    {
        getSampleJoyStick(&vx, &vy);
        int xJoyStick = vx; //printf("JoyStick xValue: %d\n", xJoyStick);
        switch (carState)
        {
        case REST:
            if (xJoyStick >= 10000)
            {
                carState = RIGHT;
                bug = true;
            }
            else if (xJoyStick <= 6000)
            {
                carState = LEFT;
                bug = true;
            }
            else if(bug)
            {
            Graphics_fillRectangle(&g_sContext, &rect);
            Graphics_drawImage(&g_sContext, &myredcar008BPP_UNCOMP, rect.xMin, 112);
            bug = false;
            }
            break;
        case RIGHT:
            if (xJoyStick > 6500 && xJoyStick < 8500) //Maybe change ranges later to 6000 to 10000 respectively
                carState = REST;
            //MOVE CAR TO THE RIGHT
            Graphics_fillRectangle(&g_sContext, &rect);
            i++;
            rect.xMin = i;
            rect.xMax = i + 15;
            glbl_carXMin = rect.xMin;
            xImage = i;
            if (xImage > 40)
            {
                rect.xMin = 40;
                rect.xMax = 55;
                xImage = 40;
                glbl_carXMin = rect.xMin;
                i = 40;
            }
            Graphics_drawImage(&g_sContext, &myredcar008BPP_UNCOMP, rect.xMin,
                               112);
            //printf("xMin: %d||xMax %d\n", rect.xMin, rect.xMax);
            //printf("RIGHT\n");
            break;
        case LEFT:
            if (xJoyStick > 6500 && xJoyStick < 8500)
                carState = REST;
            Graphics_fillRectangle(&g_sContext, &rect);
            i--;
            rect.xMin = i;
            rect.xMax = i + 15;
            glbl_carXMin = rect.xMin;
            xImage = i;
            //printf("LEFT\n");
            if (xImage < 8)
            {
                rect.xMin = 8;
                rect.xMax = 23;
                glbl_carXMin = rect.xMin;
                xImage = 8;
                i = 8;
            }
            Graphics_drawImage(&g_sContext, &myredcar008BPP_UNCOMP, rect.xMin,
                               112);
            //printf("xMin: %d||xMax %d\n", rect.xMin, rect.xMax);
            //printf("LEFT\n");
            break;
        }
    }
}

bool reset(bool reset) {
    return glbl_reset = reset;
}

void moveTree00(int code){
    static int i = 0;
    extern tImage mytree008BPP_UNCOMP; //Defined in myredcar00.c file, I don't think static is necessary
    static Graphics_Rectangle rect;

    if (glbl_reset)
    {
        glbl_treeStatus[0] = false;
        rect.yMin = 0;
        rect.yMax = 15;
        glbl_treeYMin[0] = rect.yMin;
        i = 0;
    }
    else {
        if (code == 0)
            rect.xMin = 8;
        else if (code == 1)
            rect.xMin = 24;
        else if (code == 2)
            rect.xMin = 40;

        rect.xMax = rect.xMin + 15;
        Graphics_fillRectangle(&g_sContext, &rect);
        i++;
        rect.yMin = i;
        rect.yMax = i + 15;
        glbl_treeYMin[0] = rect.yMin;
        Graphics_drawImage(&g_sContext, &mytree008BPP_UNCOMP, rect.xMin,
                           rect.yMin);
        //printf("Ymin: %d\n", rect.yMin);
        //printf("LEFT\n");
        if (rect.yMin == 129) //Or 128?
        {
            glbl_treeStatus[0] = false;
            rect.yMin = 0;
            rect.yMax = 15;
            glbl_treeYMin[0] = rect.yMin;
            i = 0;
        }
    }
}


void moveTree01(int code){
    static int i = 0;
    extern tImage mytree018BPP_UNCOMP; //Defined in myredcar00.c file, I don't think static is necessary
    //static Graphics_Rectangle rect = { 8, 0, 23, 15 }; //should probably just set yMax and yMin
    static Graphics_Rectangle rect;

    if (glbl_reset)
    {
        glbl_treeStatus[1] = false;
        rect.yMin = 0;
        rect.yMax = 15;
        glbl_treeYMin[1] = rect.yMin;
        i = 0;
    }
    else
    {

        if (code == 0)
            rect.xMin = 8;
        else if (code == 1)
            rect.xMin = 24;
        else if (code == 2)
            rect.xMin = 40;

        rect.xMax = rect.xMin + 15;
        Graphics_fillRectangle(&g_sContext, &rect);
        i++;
        rect.yMin = i;
        rect.yMax = i + 15;
        glbl_treeYMin[1] = rect.yMin;
        Graphics_drawImage(&g_sContext, &mytree018BPP_UNCOMP, rect.xMin,
                           rect.yMin);
        //printf("LEFT\n");
        if (rect.yMin == 129) //Or 128?
        {
            glbl_treeStatus[1] = false;
            rect.yMin = 0;
            rect.yMax = 15;
            glbl_treeYMin[1] = rect.yMin;
            i = 0;
        }
    }
}

void moveTree02(int code)
{
    static int i = 0;
    extern tImage mytree028BPP_UNCOMP; //Defined in myredcar00.c file, I don't think static is necessary
    static Graphics_Rectangle rect;

    if (glbl_reset)
    {
        glbl_treeStatus[2] = false;
        rect.yMin = 0;
        rect.yMax = 15;
        glbl_treeYMin[2] = rect.yMin;
        i = 0;
    }
    else
    {
        if (code == 0)
            rect.xMin = 8;
        else if (code == 1)
            rect.xMin = 24;
        else if (code == 2)
            rect.xMin = 40;

        rect.xMax = rect.xMin + 15;
        Graphics_fillRectangle(&g_sContext, &rect);
        i++;
        rect.yMin = i;
        rect.yMax = i + 15;
        //printf("LEFT\n");
        glbl_treeYMin[2] = rect.yMin;
        Graphics_drawImage(&g_sContext, &mytree028BPP_UNCOMP, rect.xMin,
                           rect.yMin);
        if (rect.yMin == 129) //Or 128?
        {
            glbl_treeStatus[2] = false;
            rect.yMin = 0;
            rect.yMax = 15;
            glbl_treeYMin[2] = rect.yMin;
            i = 0;
        }
    }
}

bool crashTree00(int code) {
    bool crash = false;
    int treeXMin = 0;
    int treeXMax = 0;
    int treeYMax = 0;
    int carXMax = 0;
    if (code == 0)
        treeXMin = 8;
    else if (code == 1)
        treeXMin = 24;
    else if (code == 2)
        treeXMin = 40;

    treeYMax = glbl_treeYMin[0] + 15;
    treeXMax = treeXMin + 15;
    carXMax = glbl_carXMin + 15;


    if((treeYMax >= 112 && treeYMax <= 142) &&
          ((glbl_carXMin >= treeXMin && glbl_carXMin <= treeXMax)||
          (carXMax >= treeXMin && carXMax <= treeXMax)))
    {
        printf("\n    Car: Xmin=%d , Xmax=%d\n", glbl_carXMin, carXMax); //For debugging
        printf("Tree00: Xmin=%d , Xmax=%d, Ymin=%d, Ymax=%d\n", treeXMin, treeXMax, glbl_treeYMin[0], treeYMax); //For debugging
        crash = true;
    }

    return crash;
}

bool crashTree01(int code)
{
    bool crash = false;
    int treeXMin = 0;

    if (code == 0)
        treeXMin = 8;
    else if (code == 1)
        treeXMin = 24;
    else if (code == 2)
        treeXMin = 40;

    int treeYMax = glbl_treeYMin[1] + 15;
    int treeXMax = treeXMin + 15;
    int carXMax = glbl_carXMin + 15;

    if ((treeYMax >= 112 && treeYMax <= 142) && ((glbl_carXMin >= treeXMin && glbl_carXMin <= treeXMax)||
       (carXMax >= treeXMin && carXMax <= treeXMax)))
    {
        printf("\n   Car: Xmin=%d , Xmax=%d\n", glbl_carXMin, carXMax); //For debugging
        printf("Tree01: Xmin=%d , Xmax=%d, Ymin=%d, Ymax=%d\n", treeXMin, treeXMax, glbl_treeYMin[1], treeYMax); //For debugging
        crash = true;
    }
    return crash;
}

bool crashTree02(int code)
{
    bool crash = false;
    int treeXMin = 0;

    if (code == 0)
        treeXMin = 8;
    else if (code == 1)
        treeXMin = 24;
    else if (code == 2)
        treeXMin = 40;

    int treeYMax = glbl_treeYMin[2] + 15;
    int treeXMax = treeXMin + 15;
    int carXMax = glbl_carXMin + 15;

    if ((treeYMax >= 112 && treeYMax <= 142) && ((glbl_carXMin >= treeXMin && glbl_carXMin <= treeXMax)
        || (carXMax >= treeXMin && carXMax <= treeXMax)))
    {
        printf("\n   Car: Xmin=%d , Xmax=%d\n", glbl_carXMin, carXMax); //For debugging
        printf("Tree02: Xmin=%d , Xmax=%d, Ymin=%d, Ymax=%d\n", treeXMin, treeXMax, glbl_treeYMin[2], treeYMax); //For debugging
        crash = true;
    }
    return crash;
}


bool getTree00Status() {
    return glbl_treeStatus[0];
}

bool getTree01Status() {
    return glbl_treeStatus[1];
}

bool getTree02Status() {
    return glbl_treeStatus[2];
}

void setTree00Status(bool status)
{
    glbl_treeStatus[0] = status;
}

void setTree01Status(bool status) {
    glbl_treeStatus[1] = status;
}

void setTree02Status(bool status) {
    glbl_treeStatus[2] = status;
}

int addScore() {
    if(glbl_treeYMin[0] == 128)
        return 1;
    else if(glbl_treeYMin[1] == 128)
        return 1;
    else if(glbl_treeYMin[2] == 128)
        return 1;
    else
        return 0;
}

int getHighScore(int userScore) {
    static int highScore = 0;
    if(userScore > highScore)
        highScore = userScore;
    return highScore;
}

int getTree00YMin(){
    return glbl_treeYMin[0];
}
int getTree00YMax(){
    return 0;
}
int getTree01YMin(){
    return glbl_treeYMin[1];
}
int getTree01YMax(){
    return 0;
}
int getTree02YMin(){
    return glbl_treeYMin[2];
}
int getTree02YMax(){
    return 0;
}

const char* intToASCII(unsigned score){
    //char buf[8] = "SCORE XX";
    const char* buf[] = {'0','0'};
    buf[0] = ((score/10) % 10) + '0';
    buf[1] = ((score   ) % 10) + '0';
    return buf;
}

void setForegroundToBlack(){
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
}

void setForegroundToWhite(){
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
}
void DrawUserScore(unsigned int curScore, unsigned int prevScore)
{
    if (curScore > prevScore)
    {
        char buf[2] = { '0', '0' };
        buf[0] = ((curScore / 10) % 10) + '0';
        buf[1] = ((curScore) % 10) + '0';
        setForegroundToWhite();
        PrintString(buf, 1, 14);
        setForegroundToBlack();
        //PrintString(intToASCII(curScore), 1, 14);
    }
}

void DrawHiScoreGameBoard(int highScore)
{ //Draws High Score on Gameboard
    char buf2[2] = {'0', '0' };
    buf2[0] = ((highScore / 10) % 10) + '0';
    buf2[1] = ((highScore) % 10) + '0';
    PrintString(buf2, 4, 14);
}

void DrawTitleScreen(){ //keep in mind that it is already drawn in LAB3_main.c
    LCDClearDisplay(GRAPHICS_COLOR_WHITE);
    PrintString("WATCH OUT!", 2, 3);
    PrintString("by", 3, 7);
    PrintString("James McCray", 4, 1);
}

void DrawStartScreen(int yPointer){ //keep in mind that it is already drawn in LAB3_main.c
    //DrawTitleScreen()
    LCDClearDisplay(GRAPHICS_COLOR_WHITE);
    PrintString("Game Menu", 0, 3); //PrintStr(text, row, col);
    PrintString("Play Game", 2, 3);
    PrintString("->", yPointer, 0);
    PrintString("How to Play", 3, 3);
    PrintString("Score History", 4, 3);
}

void DrawGameScreen(unsigned int userScore, unsigned int highScore){
    extern tImage  myredcar008BPP_UNCOMP; //Defined in myredcar00.c file

    Graphics_Rectangle scoreBoard = { 64, 0, 127, 127 }; //{xMin, yMin, xMax, yMax}
    Graphics_Rectangle gameBoard = { 8, 0, 55, 127 };
    Graphics_Rectangle LMargin = { 0, 0, 5, 127 };
    Graphics_Rectangle RMargin = { 58, 0, 63, 127 };
    LCDClearDisplay(GRAPHICS_COLOR_BLACK);

    //Draws Left Margin
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_fillRectangle(&g_sContext, &LMargin);

    //Draws Game Board
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_fillRectangle(&g_sContext, &gameBoard);

    //Draws Right Margin
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_fillRectangle(&g_sContext, &RMargin); //Draw Right Margin

    //Draws Score Board
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_NAVY);
    Graphics_fillRectangle(&g_sContext, &scoreBoard); //Draws Score Board

    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_NAVY);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    PrintString("SCORE:", 0, 8); //PrintStr(text, row, col)
    //PrintString("00", 1, 14);
    char buf1[2] = {'0','0'};
    buf1[0] = ((userScore / 10) % 10) + '0';
    buf1[1] = ((userScore) % 10) + '0';
    PrintString(buf1, 1, 14);

    PrintString("HIGH", 2, 8);
    PrintString("SCORE:", 3, 8);
    //PrintString("00", 4, 14);
    char buf2[2] = {'0','0'};
    buf2[0] = ((highScore / 10) % 10) + '0';
    buf2[1] = ((highScore) % 10) + '0';
    PrintString(buf2, 4, 14);

    //Draw Car
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK); //Clears
    //Graphics_drawImage(&g_sContext, &myredcar008BPP_UNCOMP, 8, 112);
}

void DrawInstructionScreen() {
    LCDClearDisplay(GRAPHICS_COLOR_WHITE);
    PrintString("INSTRUCTIONS", 0, 2); //PrintStr(text, row, col)
    PrintString("Use The Joystick", 1, 0); //16 Chars
    PrintString("To Move The Car", 2, 0);
    PrintString("In Order To Miss", 3, 0);
    PrintString("The Obstacles.", 4, 0);
    PrintString("Each Dodged Obs-", 5, 0);
    PrintString("tacle is a point", 6, 0);
    PrintString("BTN1-MENU", 7, 6);
}

void DrawScoreScreen(unsigned int newScore, bool on_off) {
    static unsigned int score1 = 0;
    static unsigned int score2 = 0;
    static unsigned int score3 = 0;

    if (!on_off) //Displays the Screen
    {
        /*if(newScore >= score1 && score1 == 0 && score2 == 0 && score3 == 0)
            score1 = newScore;
            printf("Score1=%d, Score2=%d, Score3=%d\n", score1, score2, score3);

        else if (newScore > score1 && score1 != 0 && score2 == 0 && score3 == 0)
        {
            score3 = score2;
            score2 = score1;
            score1 = newScore;
            printf("Score1=%d, Score2=%d, Score3=%d\n", score1, score2, score3);

        }
        else if (newScore < score1 && newScore >= score2)
        {
            score3 = score2;
            score2 = newScore;
        }
        else if (newScore > 0 && newScore > score3 && newScore < score2)
        {
            score3 = newScore;
        }
        */
    }
    else
    {
        if (newScore >= score1 && score1 == 0 && score2 == 0 && score3 == 0)
        {
            score1 = newScore;
            printf("Score1=%d, Score2=%d, Score3=%d\n", score1, score2, score3);
        }
        //else if (newScore > score1 && score1 != 0 && score2 == 0 && score3 == 0)
        else if (newScore > score1)
        {
            score3 = score2;
            score2 = score1;
            score1 = newScore;
            printf("Score1=%d, Score2=%d, Score3=%d\n", score1, score2, score3);

        }
        else if (newScore <= score1 && newScore >= score2)
        {
            score3 = score2;
            score2 = newScore;
        }
        else if (newScore > 0 && newScore > score3 && newScore < score2)
        {
            score3 = newScore;
        }

        LCDClearDisplay(GRAPHICS_COLOR_WHITE);
        PrintString("SCORES", 0, 5); //PrintStr(text, row, col)
        PrintString("SCORE 1:", 1, 0);
        PrintString("SCORE 2:", 3, 0);
        PrintString("SCORE 3:", 5, 0);
        //Write logic that converts int to char to be displayed

        char buf1[2] = { '0', '0' };
        char buf2[2] = { '0', '0' };
        char buf3[2] = { '0', '0' };

        buf1[0] = ((score1 / 10) % 10) + '0';
        buf1[1] = ((score1) % 10) + '0';
        buf2[0] = ((score2 / 10) % 10) + '0';
        buf2[1] = ((score2) % 10) + '0';
        buf3[0] = ((score3 / 10) % 10) + '0';
        buf3[1] = ((score3) % 10) + '0';

        PrintString(buf1, 2, 14);
        PrintString(buf2, 4, 14);
        PrintString(buf3, 6, 14);

        PrintString("BTN1-MENU", 7, 6);
    }
}


void DrawPauseScreen(){
    LCDClearDisplay(GRAPHICS_COLOR_WHITE);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    PrintString("  GAME PAUSED", 0, 1);
    PrintString("->", 2, 0);
    PrintString("Continue", 2, 3);
    PrintString("Menu", 3, 3);
}

void DrawGameOverScreen(int userScore) {
    LCDClearDisplay(GRAPHICS_COLOR_WHITE);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    char buf[2] = {'0','0'};
    buf[0] = ((userScore / 10) % 10) + '0';
    buf[1] = ((userScore) % 10) + '0';
    PrintString("GAME OVER", 0, 4);
    PrintString("SCORE: ", 2, 0);
    PrintString(buf, 2, 7);
    PrintString(" ", 2, 9);
    //Print actual score in FSM/
}

