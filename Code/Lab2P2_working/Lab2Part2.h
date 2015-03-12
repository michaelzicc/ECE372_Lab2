/* 
 * File:   Lab2Part2.h
 * Author: Kevin
 *
 * Created on March 5, 2015, 7:47 PM
 */

#ifndef LAB2PART2_H
#define	LAB2PART2_H

#define DISABLE 0
#define ENABLE 1
#define HIGH 1
#define LOW 0
#define TRUE 1
#define FALSE 0
#define NUMPASSWORDS 4

//Define the states used for Part 2
typedef enum {
    Start, Enter, waitForRelease, keyPress, keyRelease,
    validPW, invalidPW, validEntry, invalidEntry, setMode
} state;



#endif	/* LAB2PART2_H */

