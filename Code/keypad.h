/* 
 * File:   keypad.h
 * Author: user
 *
 * Created on February 5, 2015, 11:42 AM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

#define OUTPUT 0
#define INPUT 1
#define ENABLE 1
#define DISABLE 0
#define LOW 0
#define HIGH 1
#define P_DEBOUNCE 800
#define R_DEBOUNCE 700
#define DELAY 50


#define COL1 PORTBbits.RB8
#define COL2 PORTBbits.RB11
#define COL3 PORTBbits.RB10

#define ROW1 LATAbits.LATA0
#define ROW2 LATBbits.LATB9
#define ROW3 LATBbits.LATB2
#define ROW4 LATAbits.LATA1

#define PRESSED 0

#define CNFLAG IFS1bits.CNIF
#define CNENABLE IEC1bits.CNIE

void initKeypad(void);
char scanKeypad(void);
void setRowLow(int num);

#endif	/* KEYPAD_H */

