#include "p24FJ64GA002.h"
#include "Lab2Part2.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include <stdio.h>
#include <string.h>

#define PASSWORD_LENGTH 5	//5 because of the null character; the password must be 4 characters.
#define REAL_PW_LENGTH 4	//the password can only be 4 characters


//Adds key to the end of string, up to four characters long

void updateInputString(char key, char* string) {

    unsigned int i = 0;

    if (key != -1) {
        for (i = 0; i < PASSWORD_LENGTH; i++) {
            if (string[i] == '\0') {
                string[i] = key;
                string[i + 1] = '\0';
                i = PASSWORD_LENGTH;
            }
        }
    }
    return;
}

/*Every time a keypress returns a char succesfully, 
 * the resulting string of inputs is analyzed for invalid characters,
 * as well as detecting '**' which means the user wants to store a new password
 */
state analyzeInput(char* string) {


    CNENABLE = DISABLE;
    if (string[0] == '#') return invalidEntry;

    if (setPW == FALSE) {//Device is in "Enter" mode

        if (string[0] == '*' && string[1] == '*') {//Check for setmode
            delayMili(3);
            strcpy(string, "");
            clearLCD();
            return setMode;
        }
        if (strchr(string, '#') != NULL) {//check for invalid character '#'
            return invalidEntry;
        } else return comparePasswords(string);

    } else {//Device is in "Set Mode"
        //check for invalid character '#' or '*':
        if (strrchr(string, '#') != NULL || strrchr(string, '*') != NULL) return invalidEntry;
        //if the string is four chars long and all are valid, then the entry is complete and valid
        if (strlen(string) == REAL_PW_LENGTH) {
            delayMili(2);
            return validEntry;
        }
        //entry is still valid but is not long enough to store as a password.
        return setMode;
    }
}

/*This function is used if an entry isn't thrown out for being invalid.
 * The new entry  must be compared to existing passwords
 */
state comparePasswords(char* string) {

    CNENABLE = DISABLE;
    int i = 0;

    if (strlen(string) != REAL_PW_LENGTH) return Enter;

    if (strlen(string) == REAL_PW_LENGTH) {//If the entry is the right length

        if (passwordCount > 0) {//if there are existing passwords to compare to
            for (i = 0; i < REAL_PW_LENGTH; i++) {
                if (strcmp(passwords[i], string) == 0) {//compare using strcmp()

                    return validPW;
                }
            }
        }            //Entry isn't good or bad or invalid, but there are no stored passwords to compare to
        else {
            delayMili(10);
            strcpy(string, "");
            clearLCD();
            return Enter;
        }
    }

    return invalidPW; //if none of the above apply then the entry was valid yet not a match

}