#include "p24FJ64GA002.h"
#include "keypad.h"
#include "timer.h"

#define OUTPUT 0
#define INPUT 1
#define ENABLE 1
#define DISABLE 0
#define ENABLE2 0
#define DISABLE2 1

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void) {

    //Set Pins 2,3,6,9 as Output
    TRISAbits.TRISA0 = OUTPUT; //Pin 2
    TRISAbits.TRISA1 = OUTPUT; //Pin 3
    TRISBbits.TRISB2 = OUTPUT; //Pin 6
    TRISAbits.TRISA2 = OUTPUT; //Pin 9

    //Set Pins 17,21,22 as Input
    TRISBbits.TRISB8 = INPUT; //Pin 17
    TRISBbits.TRISB10 = INPUT; //Pin 21
    TRISBbits.TRISB11 = INPUT; //Pin 22

    //Turn on Open Drain Collection for Outputs
    ODCAbits.ODA0 = ENABLE; //Pin 2
    ODCAbits.ODA1 = ENABLE; //Pin 3
    ODCBbits.ODB2 = ENABLE; //Pin 6
    ODCAbits.ODA2 = ENABLE; //Pin 9

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

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed.
 */
char scanKeypad(void) {
    char key;

    //Disable all rows
    LATAbits.LATA0 = DISABLE2;
    LATAbits.LATA1 = DISABLE2;
    LATBbits.LATB2 = DISABLE2;
    LATAbits.LATA2 = DISABLE2;

    /*
    //Turn on Open Drain Collection for Outputs
    ODCAbits.ODA0 = DISABLE; //Pin 2
    ODCAbits.ODA1 = DISABLE; //Pin 3
    ODCBbits.ODB2 = DISABLE; //Pin 6
    ODCAbits.ODA2 = DISABLE; //Pin 9
     */

    //turn on row(pin2)
    //ODCAbits.ODA0 = ENABLE; //Pin 2
    LATAbits.LATA0 = ENABLE2;

    if (PORTBbits.RB8 == 0 && !(PORTBbits.RB10 == 0) && !(PORTBbits.RB11 == 0))
        key = '1';
    else if (PORTBbits.RB10 == 0 && !(PORTBbits.RB11 == 0) && !(PORTBbits.RB8 == 0))
        key = '3';
    else if (PORTBbits.RB11 == 0 && !(PORTBbits.RB10 == 0) && !(PORTBbits.RB8 == 0))
        key = '2';

    //turn off row(pin2) turn on row(pin3)
    //ODCAbits.ODA0 = DISABLE; //Pin 2
    //ODCAbits.ODA1 = ENABLE; //Pin 3
    LATAbits.LATA0 = DISABLE2;
    LATAbits.LATA1 = ENABLE2;
    if (PORTBbits.RB8 == 0 && !(PORTBbits.RB10 == 0) && !(PORTBbits.RB11 == 0))
        key = '*';
    else if (PORTBbits.RB10 == 0 && !(PORTBbits.RB11 == 0) && !(PORTBbits.RB8 == 0))
        key = '#';
    else if (PORTBbits.RB11 == 0 && !(PORTBbits.RB10 == 0) && !(PORTBbits.RB8 == 0))
        key = '0';

    //turn off row(pin3) turn on row(pin6)
    //ODCAbits.ODA1 = DISABLE; //Pin 3
    //ODCBbits.ODB2 = ENABLE; //Pin 6
    LATAbits.LATA1 = DISABLE2;
    LATBbits.LATB2 = ENABLE2;
    if (PORTBbits.RB8 == 0 && !(PORTBbits.RB10 == 0) && !(PORTBbits.RB11 == 0))
        key = '7';
    else if (PORTBbits.RB10 == 0 && !(PORTBbits.RB11 == 0) && !(PORTBbits.RB8 == 0))
        key = '9';
    else if (PORTBbits.RB11 == 0 && !(PORTBbits.RB10 == 0) && !(PORTBbits.RB8 == 0))
        key = '8';

    //turn off row(pin6) turn on row(pin9)
    // ODCBbits.ODB2 = DISABLE; //Pin 6
    //ODCAbits.ODA2 = ENABLE; //Pin 9
    LATBbits.LATB2 = DISABLE2; //Pin 6
    LATAbits.LATA2 = ENABLE2;
    if (PORTBbits.RB8 == 0 && !(PORTBbits.RB10 == 0) && !(PORTBbits.RB11 == 0))
        key = '4';
    else if (PORTBbits.RB10 == 0 && !(PORTBbits.RB11 == 0) && !(PORTBbits.RB8 == 0))
        key = '6';
    else if (PORTBbits.RB11 == 0 && !(PORTBbits.RB10 == 0) && !(PORTBbits.RB8 == 0))
        key = '5';


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

    return key;
}
