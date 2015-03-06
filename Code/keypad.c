#include "p24FJ64GA002.h"
#include "keypad.h"
#include "timer.h"

<<<<<<< HEAD
=======
#define OUTPUT 0
#define INPUT 1
#define ENABLE 1
#define DISABLE 0
#define LOW 0
#define HIGH 1
#define DEBOUNCE 15000
#define DELAY 5000
>>>>>>> bde5e23853fc0ab5c8cf75ffda4a1416d9d90118


#define COL1 PORTBbits.RB8
#define COL2 PORTBbits.RB11
#define COL3 PORTBbits.RB10

#define PRESSED 0
/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void) {

    //Set Pins 2,3,6,9 as Output
    TRISAbits.TRISA0 = OUTPUT; //Pin 2
    TRISAbits.TRISA1 = OUTPUT; //Pin 3
    TRISBbits.TRISB2 = OUTPUT; //Pin 6
    TRISBbits.TRISB9 = OUTPUT; //Pin 18

    //Set Pins 17,21,22 as Input
    TRISBbits.TRISB8 = INPUT; //Pin 17
    TRISBbits.TRISB10 = INPUT; //Pin 21
    TRISBbits.TRISB11 = INPUT; //Pin 22

    //Turn on Open Drain Collection for Outputs
    ODCAbits.ODA0 = ENABLE; //Pin 2
    ODCAbits.ODA1 = ENABLE; //Pin 3
    ODCBbits.ODB2 = ENABLE; //Pin 6
    ODCBbits.ODB9 = ENABLE; //Pin 18

    // DO WE NEED THIS?
    //change from analog to digital
    AD1PCFGbits.PCFG0 = 1;  //Pin 2
    AD1PCFGbits.PCFG1 = 1;  //Pin 3
    AD1PCFGbits.PCFG4 = 1;  //Pin 6

    //Enable interrupts for the whole board
    IEC1bits.CNIE = ENABLE;

    //Put the interrupt flag down
    IFS1bits.CNIF = 0;

    //Enable pull up resistors for the Inputs
    CNPU2bits.CN22PUE = ENABLE; //Pin 17
    CNPU2bits.CN16PUE = ENABLE; //Pin 21
    CNPU1bits.CN15PUE = ENABLE; //Pin 22

    //Enable Interrupts for the inputs
    CNEN2bits.CN22IE = ENABLE; //Pin 17
    CNEN2bits.CN16IE = ENABLE; //Pin 21
    CNEN1bits.CN15IE = ENABLE; //Pin 22



}

<<<<<<< HEAD
void setRowLow(int num){
//num = 0 sets all rows to high
//num = 1, 2, 3, or 4 sets the corresponding row to low and all other rows to high
//num = 5 sets all rows to low
    switch(num) {
        case 0://turn off all rows
            ROW1 = HIGH; //row 1
            ROW4 = HIGH; //row 4
            ROW3 = HIGH; //row 3
            ROW2 = HIGH; //row 2
            break;
        case 1:
            ROW1 = LOW; //row 1
            ROW4 = HIGH; //row 4
            ROW3 = HIGH; //row 3
            ROW2 = HIGH; //row 2
            break;
        case 2:
            ROW1 = HIGH; //row 1
            ROW4 = HIGH; //row 4
            ROW3 = HIGH; //row 3
            ROW2 = LOW; //row 2
            break;
        case 3:
            ROW1 = HIGH; //row 1
            ROW4 = HIGH; //row 4
            ROW3 = LOW; //row 3
            ROW2 = HIGH; //row 2
            break;
        case 4:
            ROW1 = HIGH; //row 1
            ROW4 = LOW; //row 4
            ROW3 = HIGH; //row 3
            ROW2 = HIGH; //row 2
            break;
        case 5://Turn on all Rows
            ROW1 = LOW; //row 1
            ROW4 = LOW; //row 4
            ROW3 = LOW; //row 3
            ROW2 = LOW; //row 2
            break;
    }
    delayUs(DELAY);
=======
void enableRow(int num){

    if (num == 0) {//turn off all rows
        LATAbits.LATA0 = HIGH; //row 1
        LATAbits.LATA1 = HIGH; //row 4
        LATBbits.LATB2 = HIGH; //row 3
        LATBbits.LATB9 = HIGH; //row 2
    } else if (num == 1) {
        LATAbits.LATA0 = LOW; //row 1
        LATAbits.LATA1 = HIGH; //row 4
        LATBbits.LATB2 = HIGH; //row 3
        LATBbits.LATB9 = HIGH; //row 2
    } else if (num == 2) {
        LATAbits.LATA0 = HIGH; //row 1
        LATAbits.LATA1 = HIGH; //row 4
        LATBbits.LATB2 = HIGH; //row 3
        LATBbits.LATB9 = LOW; //row 2
    } else if (num == 3) {
        LATAbits.LATA0 = HIGH; //row 1
        LATAbits.LATA1 = HIGH; //row 4
        LATBbits.LATB2 = LOW; //row 3
        LATBbits.LATB9 = HIGH; //row 2
    } else if (num == 4) {
        LATAbits.LATA0 = HIGH; //row 1
        LATAbits.LATA1 = LOW; //row 4
        LATBbits.LATB2 = HIGH; //row 3
        LATBbits.LATB9 = HIGH; //row 2
    }
    else if (num == 5) {
        LATAbits.LATA0 = LOW; //row 1
        LATAbits.LATA1 = LOW; //row 4
        LATBbits.LATB2 = LOW; //row 3
        LATBbits.LATB9 = LOW; //row 2
    }
       delayUs(5000);
>>>>>>> bde5e23853fc0ab5c8cf75ffda4a1416d9d90118
    return;

}
/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed.
 */
char scanKeypad(void) {
    char key = -1;
<<<<<<< HEAD
    
//Disable interrupts for the whole board while scanning
    CNENABLE = DISABLE;

    setRowLow(1);
=======
    //int indicator = 0;
	//Disable interrupts for the whole board while scanning
    IEC1bits.CNIE = DISABLE;

    //Disable all rows
    enableRow(0);

    enableRow(1);
>>>>>>> bde5e23853fc0ab5c8cf75ffda4a1416d9d90118

    if(COL1 == LOW){
        key = '1';
    }
    else if (COL2 == LOW){
        key = '2';
    }
    else if (COL3 == LOW){
        key = '3';
    }

<<<<<<< HEAD
    setRowLow(2);
=======
    enableRow(2);
>>>>>>> bde5e23853fc0ab5c8cf75ffda4a1416d9d90118
    if (COL1 == LOW) {
        key = '4';
    } else if (COL2 == LOW) {
        key = '5';
    } else if (COL3 == LOW) {
        key = '6';
    }

<<<<<<< HEAD
    setRowLow(3);
=======
    enableRow(3);
>>>>>>> bde5e23853fc0ab5c8cf75ffda4a1416d9d90118
    if (COL1 == LOW) {
        key = '7';
    } else if (COL2 == LOW) {
        key = '8';
    } else if (COL3 == LOW) {
        key = '9';
    }
<<<<<<< HEAD

    setRowLow(4);
    if (COL1 == LOW) {
        key = '*';
    } else if (COL2 == LOW) {
        key = '0';
    } else if (COL3 == LOW) {
        key = '#';
    }
=======

    enableRow(4);
    if (COL1 == LOW) {
        key = '*';
    } else if (COL2 == LOW) {
        key = '0';
    } else if (COL3 == LOW) {
        key = '#';
    }



    enableRow(5);


>>>>>>> bde5e23853fc0ab5c8cf75ffda4a1416d9d90118

    setRowLow(5);

<<<<<<< HEAD
    CNENABLE = ENABLE; //Re-Enable interrupts for the whole board

    CNFLAG = LOW;//Put the interrupt flag down
=======
	//Re-Enable interrupts for the whole board
    IEC1bits.CNIE = ENABLE;

	//Put the interrupt flag down
    IFS1bits.CNIF = 0;

    /*
                    Keypad Pins		  PIC Pins
            1:		3+2				17+2
            2:		1+2				22+2
            3:		5+2				21+2
            4:		3+7				17+9
            5:		1+7				22+9
            6:		5+7				21+9
            7:		3+6				17+6
            8:		1+6				22+6
            9:		5+6				21+6
     *:		3+4				17+3
            0:		1+4				22+3
            #:		5+4				21+3
     */
>>>>>>> bde5e23853fc0ab5c8cf75ffda4a1416d9d90118

    return key;
}

