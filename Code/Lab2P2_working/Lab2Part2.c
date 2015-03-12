// ******************************************************************************************* //
// File:         lab2p1.c
// Date:         Wednesday, March 11, 2015
// Authors:      Michael Ziccarelli, Noel Hagos Teku, Kevin T Gilliam, 
//					Devin John Slack, Reydesel Alejandro Cuevas
//
// Description:  4-character-password program for LCD and keypad
//
// ******************************************************************************************* //

#include "p24FJ64GA002.h"
#include "Lab2Part2.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include <stdio.h>
#include <string.h>

#define PASSWORD_LENGTH 5	//5 because of the null character; the password must be 4 characters.
#define REAL_PW_LENGTH 4	//the password can only be 4 characters
#define TIME_DELAY1 3
#define TIME_DELAY2 5
#define TIME_DELAY3 15

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
        FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)

_CONFIG2(IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
        IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT)

volatile state currState = Start;
volatile state nextState = Start;
volatile int setPW = FALSE;
volatile int pressed = FALSE;
volatile int passwordCount = 0;
volatile char passwords[NUMPASSWORDS][5];

void _ISR _CNInterrupt(void) {

    CNFLAG = LOW;
    if (!pressed) nextState = keyPress;
    if (pressed) nextState = keyRelease;

    return;
}

int main(void) {

    char key = -1;
    char lastFourKeys[PASSWORD_LENGTH] = "";

    int i = 0;

    initLCD();
    initKeypad();
    initTimer2();
    initTimer1();


    for (i = 0; i < REAL_PW_LENGTH; i++) {
        strcpy(passwords[i], "");
    }

    while (1) {
        switch (currState) {


            case Start:
                delayMili(TIME_DELAY2);
                nextState = Enter;
                break;

            case waitForRelease:
                break;

            case Enter:
                moveCursorLCD(0, 0);
                if (setPW == FALSE) printStringLCD("Enter   ");
                else printStringLCD("SET   ");
                moveCursorLCD(1, 0);
                printStringLCD(lastFourKeys);
                CNENABLE = ENABLE;
                break;

            case validPW:
                CNENABLE = DISABLE;
                delayMili(TIME_DELAY1);
                clearLCD();
                moveCursorLCD(0, 0);
                printStringLCD("Good    ");
                delayMili(TIME_DELAY3);
                moveCursorLCD(1, 0);
                strcpy(lastFourKeys, "");
                nextState = Enter;
                break;

            case invalidPW:
                CNENABLE = DISABLE;
                delayMili(TIME_DELAY1);
                clearLCD();
                moveCursorLCD(0, 0);
                printStringLCD("Bad     ");
                delayMili(TIME_DELAY3);
                moveCursorLCD(1, 0);
                strcpy(lastFourKeys, "");
                nextState = Enter;
                break;

            case validEntry:
                CNENABLE = DISABLE;
                setPW = FALSE;
                strcpy((char*) passwords[ passwordCount % NUMPASSWORDS ], lastFourKeys);
                passwordCount++;
                strcpy(lastFourKeys, "");
                clearLCD();
                moveCursorLCD(0, 0);
                printStringLCD("Valid ");
                delayMili(TIME_DELAY3);
                nextState = Enter;
                break;

            case invalidEntry:
                CNENABLE = DISABLE;
                setPW = FALSE;
                clearLCD();
                strcpy(lastFourKeys, "");
                moveCursorLCD(0, 0);
                printStringLCD("Invalid ");
                delayMili(TIME_DELAY3);
                nextState = Enter;
                break;

            case setMode:
                CNENABLE = ENABLE;
                if (setPW == FALSE) {
                    strcpy(lastFourKeys, "");
                    setPW = TRUE;
                }
                moveCursorLCD(0, 0);
                printStringLCD("SET MODE");
                moveCursorLCD(1, 0);
                printStringLCD(lastFourKeys);
                break;

            case keyPress:
                pressed = TRUE;
                delayUs(DEBOUNCE);
                moveCursorLCD(1, 0);
                printStringLCD(lastFourKeys);
                key = scanKeypad();
                if (key == -1) {
                    nextState = keyRelease;
                    break;
                }
                updateInputString(key, lastFourKeys);
                nextState = waitForRelease;
                break;

            case keyRelease:
                pressed = FALSE;
                delayUs(DEBOUNCE);
                moveCursorLCD(1, 0);
                printStringLCD(lastFourKeys);
                nextState = analyzeInput(lastFourKeys);
                break;

        }
        currState = nextState;
    }
    return 0;
}

