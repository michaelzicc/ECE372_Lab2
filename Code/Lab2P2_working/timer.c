#include "p24FJ64GA002.h"
#include "Lab2Part2.h"
#include "timer.h"
#include "lcd.h"

volatile unsigned int T = 0;

void _ISR _T2Interrupt() {

    TIMER2FLAG = LOW; //Lower the flag

}

void _ISR _T1Interrupt() {

    TIMER1FLAG = 0; //Lower the flag
    TMR1 = 0;
    T++;

}

void initTimer1() {
    // This is so that we do not have to calculate PR by hand.
    unsigned int prVal = (FCY * .025) / 256.0 - 1;
    PR1 = prVal;
    T1CONbits.TCKPS = 0b11; //prescalar 256
    IEC0bits.T1IE = 1; // Enable the interrupt
    IFS0bits.T1IF = 0; // Put the interrupt flag down
    T1CONbits.TON = 1; // Turn the timer 1 on
    //    T1CONbits.TCS = 1;

}

void initTimer2() { //initializes Timer 2 :turning it on, setting its prescalar and interrupt flag,
    //setting its PR value, and setting counter to 0.
    T2CONbits.TCKPS = 0; //timer2's prescalar is set to 0
    TIMER2FLAG = LOW; //interrupt flag put down
    TIMER2ONOFF = OFF; //timer2 is turned off
    TMR2 = 0; //timer2 counter is set to 0.
    PR2 = ONE_MICROSECOND;
}

void delayMili(unsigned int delay) {
    T = 0;

    TIMER1ONOFF = ON;

    while (T <= delay * 4) {
    }

    TIMER1ONOFF = OFF;

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
