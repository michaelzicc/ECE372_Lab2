// ******************************************************************************************* //
// File:         lab2p1.c
// Date:         Wednesday, March 11, 2015
// Authors:      Michael Ziccarelli, Noel Hagos Teku, Kevin T Gilliam, 
//					Devin John Slack, Reydesel Alejandro Cuevas
//
// Description:  Program that displays keypad presses on an LCD
//
// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include <stdio.h>



#define DISABLE 0
#define ENABLE 1
#define MAX_SECOND_ROW 16
#define MAX_FIRST_ROW 8
#define RESET 0

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
        FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)

_CONFIG2(IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
        IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT)


typedef enum { //declaring state variable to implement to switch statement
    start, wait1, press_debounce, wait2, release_debounce, write_char
} state;

//volatile variable declaration and initialization
volatile state curr = start; //curr is used to change into different states
volatile state prev = start; // prev keeps track of the current state if we need to go back.
volatile state next = start; //next keeps track of which state curr will be next

int main(void) {
    char a;
    int counter = RESET;
    initLCD();
    initKeypad();

    while (1) {
        switch (curr) {
            case start:
                curr = wait1;
                break;

            case wait1:
                if (next == press_debounce) {
                    curr = next;
                    next = wait1;
                }
                break;

            case press_debounce: //I put this stuff back in here from the scan state while debugging then couldn't remember how it was originally... but it seems to work anyway
                delayUs(P_DEBOUNCE);
                curr = wait2;
                a = scanKeypad();
                if (a == -1)
                    //  prev = press_debounce;
                    curr = wait1;
                break;

            case wait2: break;

            case release_debounce:
                delayUs(R_DEBOUNCE);
                curr = write_char;
                break;

            case write_char:
                curr = wait1; //set the state first so if a button is pressed, it can go straight to press_debounce from the ISR
                printCharLCD(a);
                counter = counter + 1;
                if (counter == MAX_FIRST_ROW) { //if courser reached the end of the first row
                    moveCursorLCD(1, 0); //move the cursor to the beginning of the second row
                } else if (counter == MAX_SECOND_ROW) { //if cursor reached the end of the second row
                    counter = RESET; //reset cursor position counter
                    moveCursorLCD(0, 0); //move the cursor back to the beginning of the first row
                }
                break;

            default: curr = wait1;
        }
    }

    return 0;
}

void _ISR _CNInterrupt(void) { //used to observe change interrupts

    CNFLAG = LOW; //put the flag down

    if (curr == start || curr == wait1)curr = press_debounce;

    else if (curr == wait2) curr = release_debounce;

    else if (curr == release_debounce) next = press_debounce; //don't change curr because we still need to print the character

    else if (curr == write_char) curr = press_debounce;

    return;
}