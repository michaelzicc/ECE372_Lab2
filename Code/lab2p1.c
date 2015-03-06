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


<<<<<<< HEAD

=======
#define COLUMN1 PORTBbits.RB8 
#define COLUMN2 PORTBbits.RB10
#define COLUMN3 PORTBbits.RB11
>>>>>>> bde5e23853fc0ab5c8cf75ffda4a1416d9d90118
#define DISABLE 0
#define ENABLE 1


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

//variables used to keep track of time
volatile unsigned int minutes = 0;
volatile unsigned int seconds = 0;
volatile unsigned int FF = 0;

int main(void) {
    char a;
    int counter = 0;
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
<<<<<<< HEAD

            case press_debounce://I put this stuff back in here from the scan state while debugging then couldn't remember how it was originally... but it seems to work anyway
                delayUs(P_DEBOUNCE);
=======
            case press_debounce:
                delayUs(DEBOUNCE);
                //curr = wait2;
               // a = scanKeypad();
                //if (a == -1)
                  //  curr = wait1;
                curr = scan;
                break;
               case scan:
                curr = wait2; //set the state first so if the button is released, it can go straight to release_debounce from the ISR
                a = scanKeypad();
                 if (a == -1)
                    curr = wait1;
                break;
            case wait2:
                //      IFS1bits.CNIF = 0;
>>>>>>> bde5e23853fc0ab5c8cf75ffda4a1416d9d90118
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
                if(a!=-1)
                {
                printCharLCD(a);
<<<<<<< HEAD
                counter = counter + 1;
                if (counter == 8) {
                    moveCursorLCD(1, 0);
                } else if (counter == 16) {
                    counter = 0;
                    moveCursorLCD(0, 0);
=======
                counter = counter +1;
                if(counter == 9)
                {
                    moveCursorLCD(1,0);
                }
                else if(counter == 17)
                {
                    counter = 0;
                    moveCursorLCD(0,0);
                }
>>>>>>> bde5e23853fc0ab5c8cf75ffda4a1416d9d90118
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