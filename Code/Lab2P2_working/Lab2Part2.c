// ******************************************************************************************* //
// File:         lab2p1.c
// Date:
// Authors:
//
// Description:
// ******************************************************************************************* //

#include "p24FJ64GA002.h"
#include "Lab2Part2.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include <stdio.h>
#include <string.h>

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
    char lastFourKeys[5] = "";

    int i = 0;

    initLCD();
    initKeypad();
    initTimer2();
    initTimer1();


    for (i = 0; i < 4; i++) {
        strcpy(passwords[i], "");
    }

    while (1) {
        switch (currState) {


            case Start:
                delayMili(5);
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
                delayMili(3);
                clearLCD();
                moveCursorLCD(0, 0);
                printStringLCD("Good    ");
                delayMili(15);
                moveCursorLCD(1, 0);
                strcpy(lastFourKeys, "");
                nextState = Enter;
                break;

            case invalidPW:
                CNENABLE = DISABLE;
                delayMili(3);
                clearLCD();
                moveCursorLCD(0, 0);
                printStringLCD("Bad     ");
                delayMili(15);
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
                delayMili(15);
                nextState = Enter;
                break;

            case invalidEntry:
                CNENABLE = DISABLE;
                setPW = FALSE;
                clearLCD();
                strcpy(lastFourKeys, "");
                moveCursorLCD(0, 0);
                printStringLCD("Invalid ");
                delayMili(15);
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

void updateInputString(char key, char* string) {
//Adds key to the end of string, up to four characters long
    unsigned int i = 0;

    if (key != -1) {
        for (i = 0; i < 5; i++) {
            if (string[i] == '\0') {
                string[i] = key;
                string[i + 1] = '\0';
                i = 5;
            }
        }
    }
    return;
}

state analyzeInput(char* string) {
/*Every time a keypress returns a char succesfully, 
 * the resulting string of inputs is analyzed for invalid characters,
 * as well as detecting '**' which means the user wants to store a new password
 */

    CNENABLE = DISABLE;
    if (string[0] == '#') return invalidEntry;

    if (setPW == FALSE) {//Device is in "Enter" mode

        if (string[0] == '*' && string[1] == '*') {//Check for setmode
            delayMili(3);
            strcpy(string, "");
            clearLCD();
            return setMode;
        }
        if (strchr(string, '#') != NULL) {//check for invalid character '#'
            return invalidEntry;
        } else return comparePasswords(string);

    } else {//Device is in "Set Mode"
        //check for invalid character '#' or '*':
        if (strrchr(string, '#') != NULL || strrchr(string, '*') != NULL) return invalidEntry;
 //if the string is four chars long and all are valid, then the entry is complete and valid
        if (strlen(string) == 4) {
            delayMili(2);
            return validEntry;
        }
        //entry is still valid but is not long enough to store as a password.
        return setMode;
    }
}

state comparePasswords(char* string) {
/*This function is used if an entry isn't thrown out for being invalid.
 * The new entry  must be compared to existing passwords
 */
    CNENABLE = DISABLE;
    int i = 0;

    if (strlen(string) != 4) return Enter;

    if (strlen(string) == 4) {//If the entry is the right length

        if (passwordCount > 0) {//if there are existing passwords to compare to
            for (i = 0; i < 4; i++) {
                if (strcmp(passwords[i], string) == 0) {//compare using strcmp()

                    return validPW;
                }
            }
        }
        //Entry isn't good or bad or invalid, but there are no stored passwords to compare to
        else {
            delayMili(10);
            strcpy(string, "");
            clearLCD();
            return Enter;
        }
    }

    return invalidPW;//if none of the above apply then the entry was valid yet not a match

}