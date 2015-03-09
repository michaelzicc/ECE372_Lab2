/*
 * File:   timer.h
 * Author:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#ifndef INITTIMER_H
#define	INITTIMER_H

#define FCY  14745600
#define ONE_MICROSECOND 1473
#define LOW 0
#define HIGH 1
#define ON 1
#define OFF 0
#define TIMER2FLAG IFS0bits.T2IF
#define TIMER1FLAG IFS0bits.T1IF
#define TIMER2ONOFF T2CONbits.TON
#define TIMER1ONOFF T1CONbits.TON
#define ENABLE 1
#define DISABLE 0
#define CNENABLE IEC1bits.CNIE

void initTimer2(); 
void initTimer1();
void delayUs(unsigned int delay);

#endif	/* INITTIMER_H */


