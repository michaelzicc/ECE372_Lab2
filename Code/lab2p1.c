// ******************************************************************************************* //
// File:         lab2p1.c
// Date:         
// Authors:      
//
// Description: 
// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include <stdio.h>

#define DEBOUNCE 15000

#define COLUMN1 PORTBbits.RB9 
#define COLUMN2 PORTBbits.RB10
#define COLUMN3 PORTBbits.RB11


_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
        FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)

_CONFIG2(IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
        IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT)


typedef enum statee { //declaring state variable to implement to switch statement
    start, wait1, press_debounce, scan, wait2, release_debounce, write_char
} state;

//volatile variable declaration and initialization
volatile state curr = start; //curr is used to change into different states
volatile state prev = start; // prev keeps track of the current state if we need to go back.
volatile state next = start; //next keeps track of which state curr will be next

//variables used to keep track of time
volatile unsigned int minutes = 0;
volatile unsigned int seconds = 0;
volatile unsigned int FF = 0;

int main(void) {
    char a;
    initLCD();

    while (1) {
        switch (curr) {
            case start:
                curr = wait1;
                break;
            case wait1:
                curr = wait1;
                break;
            case press_debounce:
                delayUs(DEBOUNCE);
                curr = scan;
                break;
            case scan:
                a = scanKeypad();
                curr = wait2;
                break;
            case release_debounce:
                delayUs(DEBOUNCE);
                curr = write_char;
                break;
            case write_char:
                printCharLCD(a);
                break;
        }
    }

    return 0;
}

void _ISR _CNInterrupt(void) { //used to observe change interrupts
    IFS1bits.CNIF = 0; //put the flag down


}