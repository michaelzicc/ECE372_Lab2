

/*
 * File:   lcd.h
 * Authors:
 *
 * Created on December 31, 2014, 1:39 PM
 */
#ifndef LCD_H
#define LCD_H

void writeLCD(unsigned char word, unsigned int commandType, unsigned int delay); //writes to the LCD
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower);
void initLCD(void);
void clearLCD(void);
void moveCursorLCD(unsigned char x, unsigned char y);
void printCharLCD(char c);
void printStringLCD(const char* s);


#endif//LCD_H