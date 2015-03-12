/* 
 * File:   Lab2Part2_functions.h
 * Author: Kevin
 *
 * Created on March 5, 2015, 7:47 PM
 */

#ifndef LAB2PART2_FUNCTIONS_H
#define	LAB2PART2_FUNCTIONS_H

#define DISABLE 0
#define ENABLE 1
#define HIGH 1
#define LOW 0
#define TRUE 1
#define FALSE 0
#define NUMPASSWORDS 4


void delayMili(unsigned int delay);
void updateInputString(char key, char* string);
state analyzeInput(char* string);
state comparePasswords(char* string);

#endif	/* LAB2PART2_FUNCTIONS_H */

