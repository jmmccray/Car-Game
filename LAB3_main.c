/*
 * You can add more of your own developed *.h files here.
 * However, you are not allowed to include driverlib, grlib, stdio or any other library in this file.
 */

#include <LED_HAL.h>
#include <Buttons_HAL.h>
#include <Timer_HAL.h>
#include <Display_HAL.h>
#include <ADC_HAL.h>
#include <my_HAL.h>

#define OPENING_WAIT 10000      // 10 seconds or 10000 ms
#define OPENING_WAIT2 3000      // 3 seconds
#define CAR_TREE_SPEED 50       // 50ms
#define TREE_CALL 3700          // 3300ms
#define SCORE_DELAY 60          // 60ms
#define GAME_OVER_DELAY 3000    // 3 seconds


void ScreensFSM()
{
    static enum states {INCEPTION, OPENING, MENU, PLAY, INSTRUCTIONS, SCORE, PAUSE, ENDGAME} state = INCEPTION;
    static OneShotSWTimer_t OST;  // Opening Screen
    static OneShotSWTimer_t OST1; // Used for car & tree speed
    static OneShotSWTimer_t OST2; // Used for tree call
    static OneShotSWTimer_t OST3; // Actually can delet this
    static OneShotSWTimer_t OST4; // Used to display ENDGAME for 3s
    int static yPointer = 2;
    InitOneShotSWTimer(&OST, TIMER32_1_BASE, OPENING_WAIT2);
    InitOneShotSWTimer(&OST1, TIMER32_1_BASE, CAR_TREE_SPEED);
    InitOneShotSWTimer(&OST2, TIMER32_1_BASE, TREE_CALL);
    InitOneShotSWTimer(&OST3, TIMER32_1_BASE, SCORE_DELAY);
    InitOneShotSWTimer(&OST4, TIMER32_1_BASE, GAME_OVER_DELAY);

    // Set the default outputs
    bool drawOpening = false;
    bool drawMenu = false;
    bool drawGame = false;
    bool drawInstructions = false;
    bool drawScores = false;
    bool drawPause = false;
    bool drawEndGame = false;

    int static tree00_code;
    int static tree01_code;
    int static tree02_code;

    unsigned static int curScore = 0;
    unsigned static int prevScore = 0;
    unsigned static int highScore = 0;
    unsigned static int curScoreHold = 0;


    switch (state)
    {
    case INCEPTION:
        StartOneShotSWTimer(&OST); //StartOneShot3sTimer();
        // State transition
        state = OPENING; //uncomment

        // The output(s) that are affected in this transition
        drawOpening = true;
    break;
    case OPENING:
        if (OneShotSWTimerExpired(&OST)) // if(OneShot3sTimerExpired())
        {
            // State transition
            state = MENU;

            // The output(s) that are affected in this transition
            drawMenu = true;
        }
    break;
    case MENU:
        moveGameMenuArrow(&yPointer);
         if (Booster_Joystick_Pressed())
        {
           if(yPointer == 2)
          {
              state = PLAY;
              StartOneShotSWTimer(&OST1);
              StartOneShotSWTimer(&OST2);
              curScore = 0;
              drawGame = true;
              tree00_code = rand() % 3;
              setTree00Status(true);
          }
           else if(yPointer == 3)
          {
              state = INSTRUCTIONS;
              drawInstructions = true;
          }
           else if(yPointer == 4)
          {
              state = SCORE;
              drawScores = true;
          }
        }
    break;
    case PLAY:

        if(Booster_Top_Button_Pushed())
        {
                   state = PAUSE;
                   drawPause = true;
                   yPointer = 2;
        }

        if(OneShotSWTimerExpired(&OST1))
        {
            if(getTree00Status())
                moveTree00(tree00_code);
            if(getTree01Status())
                moveTree01(tree01_code);
            if(getTree02Status())
                moveTree02(tree02_code);
            moveCar();


            //CAR CRASH LOGIC
            if(crashTree00(tree00_code) || crashTree01(tree01_code) || crashTree02(tree02_code))
            {
                state = ENDGAME;
                drawEndGame = true;
                reset(true);
                moveCar();
                moveTree00(tree00_code);
                moveTree01(tree01_code);
                moveTree02(tree02_code);
                reset(false);
                StartOneShotSWTimer(&OST4);
            }

            //CHECKS IF A POINT SHOULD BE ADDED
            if (getTree00YMin() == 128)
                curScore++;
            else if (getTree01YMin() == 128)
                curScore++;
            else if (getTree02YMin() == 128)
                curScore++;

            DrawUserScore(curScore, prevScore);
            DrawScoreScreen(curScore, false);


            prevScore = curScore;
            StartOneShotSWTimer(&OST1);
        }


         if (OneShotSWTimerExpired(&OST2))
        {
            if (!getTree00Status())
            {
                //tree00_code = rand() % 3;
                tree00_code = randomInt();
                setTree00Status(true);
                StartOneShotSWTimer(&OST2);
            }
            else if (!getTree01Status())
            {
                //tree01_code = rand() % 3;
                tree01_code = randomInt();
                setTree01Status(true);
                StartOneShotSWTimer(&OST2);
            }
            else if (!getTree02Status())
            {
                //tree02_code = rand() % 3;
                tree02_code = randomInt();
                setTree02Status(true);
                StartOneShotSWTimer(&OST2);
            }
         }



    break;
    case INSTRUCTIONS:
        if(Booster_Top_Button_Pushed()) //Booster_Top_Button_Pushed(), defined by Prof in Buttons_HAL.c file
        {
            yPointer = 3;
            state = MENU;
            drawMenu = true;
        }
    break;
    case SCORE:
        if(Booster_Top_Button_Pushed())
        {
            yPointer = 4;
            state = MENU;
            drawMenu = true;
        }
    break;
    case PAUSE:
       moveGamePauseArrow(&yPointer);
         if (Booster_Joystick_Pressed())
        {
            if (yPointer == 2)
            {
                state = PLAY;
                curScoreHold = curScore;
                drawGame = true;
                InitOneShotSWTimer(&OST2, TIMER32_1_BASE, TREE_CALL);
                StartOneShotSWTimer(&OST2);
            }
            else if (yPointer == 3)
            {
                state = MENU;
                drawMenu = true;
            }
        }
    break;
    case ENDGAME:
        while(!OneShotSWTimerExpired(&OST4)){}
            state = MENU;
            drawMenu = true;
    break;
    } //End of Switch-Case

    //OUTPUT MACHINE
    if (drawOpening)
        DrawTitleScreen();

    if (drawMenu)
    {
        DrawStartScreen(yPointer);
        highScore = getHighScore(curScore);
    }

    if (drawGame)
    {
        DrawGameScreen(curScoreHold, highScore);
        pausedCar();
    }

    if (drawInstructions)
        DrawInstructionScreen();

    if (drawScores)
    {
        //printf("Current Score: %d\n", curScore); For Debugging
        DrawScoreScreen(curScore, true);
    }

    if (drawPause)
        DrawPauseScreen();

    if (drawEndGame)
    {
        DrawGameOverScreen(curScore);
    }

}

int main(void) {

    WDT_A_hold(WDT_A_BASE);

    BSP_Clock_InitFastest();
    InitGraphics();
    InitHWTimers();
    InitButtons();
    InitLEDs();
    initADC_Multi();
    initJoyStick();
    startADC();

    while (1) {
        // Do not delete this statement. We will use this function to check if your program does not block on anything.
        if (Booster_Bottom_Button_Pushed())
            Toggle_Launchpad_Right_Red_LED();

        ScreensFSM();
    }

}
