

/*
 * File:   lcd.c
 * Authors:
 *
 * Created on December 31, 2014, 1:39 PM
 */

#include "p24fj64ga002.h"
#include "lcd.h"
#include "timer.h"

#define LCD_DATA   LATB
#define LCD_RS  LATBbits.LATB7
#define LCD_E   LATBbits.LATB6

#define TRIS_D7  TRISBbits.TRISB15
#define TRIS_D6  TRISBbits.TRISB14
#define TRIS_D5  TRISBbits.TRISB13
#define TRIS_D4  TRISBbits.TRISB12
#define TRIS_RS  TRISBbits.TRISB7
#define TRIS_E   TRISBbits.TRISB6

#define DISPLAY_OFF 0x08 
#define CLEAR_DISPLAY 0x01
#define DISPLAY_ON 0x0C
#define ENTRY_SET 0x06
#define INIT_MESSAGE 0x03  
#define MOVE_CURSOR 0x80 
#define RETURN_HOME 0x02
#define FUNCTION_SET 0x28  
#define INIT_MESSAGE2 0x02

#define LCD_WRITE_DATA    1
#define LCD_WRITE_CONTROL 0

#define UPPER 0
#define LOWER 1

/* This function should take in a two-byte word and writes either the lower or upper
 * byte to the last four bits of LATB. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower) {

    if (lower) {
        LCD_DATA = ((LCD_DATA & 0x0FFF) | ((word & 0x0F) << 12));
    } else {
        LCD_DATA = ((LCD_DATA & 0x0FFF) | ((word & 0xF0) << 8));

    }
    LCD_RS = commandType;
    delayUs(1);
    LCD_E = 1;
    delayUs(1); //300 ns is safe
    LCD_E = 0;
    delayUs(1);
    delayUs(delayAfter);

}

/* Using writeFourBits, this function should write the two bytes of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter) {

    writeFourBits(word, commandType, delayAfter, UPPER);
    writeFourBits(word, commandType, delayAfter, LOWER);

}

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c) {

    writeLCD(c, LCD_WRITE_DATA, 52);
}

/*Initializes the LCD
 */
void initLCD(void) {

    // Setup D, RS, and E to be outputs (0).
    TRIS_RS = 0;
    TRIS_E = 0;
    TRIS_D7 = 0;
    TRIS_D6 = 0;
    TRIS_D5 = 0;
    TRIS_D4 = 0;
    delayUs(15000); //delay afterwards


    // Initilization sequence utilizes specific LCD commands before the general configuration
    // commands can be utilized. The first few initilition commands cannot be done using the
    // WriteLCD function. Additionally, the specific sequence and timing is very important.

    // Enable 4-bit interface
    writeFourBits(INIT_MESSAGE, LCD_WRITE_CONTROL, 4100, LOWER); 

    writeFourBits(INIT_MESSAGE, LCD_WRITE_CONTROL, 100, LOWER);

    writeLCD(0x32, LCD_WRITE_CONTROL, 100);

    // Function Set (specifies data width, lines, and font.
    writeLCD(FUNCTION_SET, LCD_WRITE_CONTROL, 40);



    // 4-bit mode initialization is complete. We can now configure the various LCD
    // options to control how the LCD will function.

    // Display On/Off Control
    // Turn Display (D) Off
    writeLCD(DISPLAY_OFF, LCD_WRITE_CONTROL, 40);
    // Clear Display
    writeLCD(CLEAR_DISPLAY, LCD_WRITE_CONTROL, 1640);

    // Set Increment Display, No Shift (i.e. cursor move)
    writeLCD(ENTRY_SET, LCD_WRITE_CONTROL, 40);

    // Turn Display (D) On, Cursor (C) Off, and Blink(B) Off
    writeLCD(DISPLAY_ON, LCD_WRITE_CONTROL, 40);
}

/*
 * You can use printCharLCD here. Note that every time you write a character
 * the cursor increments its position automatically.
 * Since a string is just a character array, try to be clever with your use of pointers.
 */
void printStringLCD(const char* s) {

    int i = 0;
    while (s[i] != 0) {
        printCharLCD(s[i]); //passing each character into the printChar LCD function.
        i++;
    }
}

/*
 * Clear the display.
 */
void clearLCD() {

    //passing in the clear display command four bits at a time.
    writeFourBits(CLEAR_DISPLAY, LCD_WRITE_CONTROL, 46, UPPER);
    writeFourBits(CLEAR_DISPLAY, LCD_WRITE_CONTROL, 1640, LOWER);



}

/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char x, unsigned char y) {
    unsigned char address;
    if (x == 0) //if we are in the first row
        address = 0x00; //we assign this to address

    else if (x == 1) //if we are in the second row
        address = 0x40; //we assign this to address.

    address = address + y; //we then move address horizontally based on what y is

    address = address | MOVE_CURSOR; //and we or the address with the command for the Move cursor

    writeLCD(address, LCD_WRITE_CONTROL, 44); //it is then written to th eLCD.
}

//

