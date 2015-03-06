#include "p24fj64ga002.h"
#include "timer.h"
#include "lcd.h"  
#define FCY  14745600
#define ONE_HUNDRETH_DELAY 575
#define ONE_MICROSECOND 1473
#define LOW 0
#define HIGH 1
#define ON 1
#define OFF 0
#define TIMER2FLAG IFS0bits.T2IF
#define TIMER2ONOFF T2CONbits.TON


void _ISR _T2Interrupt(){

    TIMER2FLAG = LOW;//Lower the flag

}

void initTimer2() { //initializes Timer 2 :turning it on, setting its prescalar and interrupt flag,
                     //setting its PR value, and setting counter to 0. 
    T2CONbits.TCKPS = 0; //timer2's prescalar is set to 256
    TIMER2FLAG = LOW; //interrupt flag put down
    TIMER2ONOFF = OFF; //timer2 is turned off
    TMR2 = 0; //timer2 counter is set to 0.
    PR2 = ONE_MICROSECOND;
}

void delayUs(unsigned int delay) { //function used to calculate reasonable delay in microseconds
    unsigned int elapsed = 0;

    TIMER2ONOFF = ON; //timer2 is turned on

    while (elapsed < delay) {
        TMR2 = 0; //timer2's counter set to 0.
        while (TIMER2FLAG == LOW); //remains in the while loop until interrupt flag is raised
        elapsed++;
    }
    
    TIMER2ONOFF = OFF; //timer2 is turned off

}
