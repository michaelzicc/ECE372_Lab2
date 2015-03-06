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



#define DISABLE 0
#define ENABLE 1


_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
        FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)

_CONFIG2(IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
        IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT)


typedef enum { //declaring state variable to implement to switch statement
    start, wait_for_press, press_debounce, wait_for_release, release_debounce, write_char_1, set_mode, press_debounce_2, 
	wait_for_press_2, wait_for_release_2, release_debounce_2, write_char_2, valid, invalid, good, bad, wait_2_seconds
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
	char pw[5]; //char array of length 5 for the password (5 because the null character)
	char pw_1[5], pw_2[5], pw_3[5], pw_4[5]; //stored passwords
	 
	int counter = 0;
    initLCD();
    initKeypad();

    while (1) {
        switch (curr) {
            case start: // Start in Enter Mode
				i=0;
                curr = wait_for_press;
                break;
            case wait_for_press: 
				//while(CNFLAG == LOW); //just wait for the CNFLAG to go high
				break;
			case press_debounce:
				delayUs(P_DEBOUNCE);
				curr = wait_for_release;
				a = scanKeypad();
				break;
			case wait_for_release:
				//while(CNFLAG == LOW); //just wait for the CNFLAG to go high
				break;
			case release_debounce:
				delayUs(R_DEBOUNCE);
                curr = write_char_1;
                break;
			case write_char_1:
				if(i<3) curr = wait_for_press;
				if(i==3 && checkGood()) curr = good;
				if(i==3 && !checkGood()) curr = bad;
				if(checkModeChange()) curr = set_mode;
				printCharLCD(a);
				break;
			case set_mode: // Start Set Mode
				i=0;
				curr = wait_for_press_2;
				break;
			case wait_for_press_2:
				break;
			case press_debounce_2:
				delayUs(P_DEBOUNCE);
				curr = wait_for_release_2;
				a = scanKeypad();
				break;
			case wait_for_release_2:
				break;
			case release_debounce_2:
				delayUs(R_DEBOUNCE);
                curr = write_char_2;
                break;
			case write_char_2:
				if(i<3) curr = wait_for_press_2;
				if(i==3 && checkValid()) curr = valid;
				if(i==3 && !checkValid()) curr = invalid;
				//if(checkModeChange()) curr = set_mode;
				printCharLCD(a);
				break;
			case valid:
				//print valid
				curr = wait_2_seconds;
				break;
			case invalid:
				//print invalid
				curr = wait_2_seconds;
				break;
			case good:
				//print good
				curr = wait_2_seconds;
				break;
			case bad:
				//print bad
				curr = wait_2_seconds;
				break;
			case wait_2_seconds:
				delayUs(500000);
				delayUs(500000);
				delayUs(500000);
				delayUs(500000);
				break;
				

            

            default: curr = wait_for_press;
        }
    }

    return 0;
}

void _ISR _CNInterrupt(void) { //used to observe change interrupts

    CNFLAG = LOW; //put the flag down

    if (curr == start || curr == wait_for_press)curr = press_debounce;

    else if (curr == wait2) curr = release_debounce;
    
    else if (curr == release_debounce) next = press_debounce; //don't change curr because we still need to print the character
       
    else if (curr == write_char) curr = press_debounce;
       
    return;
}