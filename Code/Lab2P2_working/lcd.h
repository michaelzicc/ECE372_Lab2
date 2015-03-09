/*
 * File:   lcd.h
 * Authors:
 *
 * Created on December 31, 2014, 1:39 PM
 */
#ifndef LCD_H
#define LCD_H

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
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delay); //writes to the LCD
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower);
void initLCD(void);
void clearLCD(void);
void moveCursorLCD(unsigned char x, unsigned char y);
void printCharLCD(char c);
void printStringLCD(const char* s);


#endif//LCD_H