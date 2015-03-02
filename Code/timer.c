#include "p24fj64ga002.h"
#include "timer.h"
#include "lcd.h"  
#define FCY  14745600
#define ONE_HUNDRETH_DELAY 575
#define ONE_MICROSECOND 14
void initTimer1() { //initializes Timer 1 :turning it on, setting its prescalar and interrupt flag,
                    //setting its PR value, and setting counter to 0.
    T1CONbits.TCKPS = 0b11; //setting timer1's prescale value to 256
    IEC0bits.T1IE = 1; //enabling interrupts for timer1
    IFS0bits.T1IF = 0; //puts down interrupt flag
    T1CONbits.TON = 1; //turns on timer1
    TMR1 = 0; //sets counter to 0
    PR1 = ONE_HUNDRETH_DELAY; //calculated delay for 1/100th of a second
}

void initTimer2() { //initializes Timer 1 :turning it on, setting its prescalar and interrupt flag,
                     //setting its PR value, and setting counter to 0. 
    T2CONbits.TCKPS = 0b11; //timer2's prescalar is set to 256
    IFS0bits.T2IF = 0; //interrupt flag put down
    T2CONbits.TON = 1; //timer2 is turned on
    TMR2 = 0; //timer2 counter is set to 0.
}

void delayUs(unsigned int delay) { //function used to calculate reasonable delay in microseconds
   
    TMR2 = 0; //timer2's counter set to 0.
    PR2 = ONE_MICROSECOND * delay; //used to calculate delay in microseconds.
    IFS0bits.T2IF = 0; //interrupt flag put down
    T2CONbits.TCKPS = 0; //prescalar value set to 0
    T2CONbits.TON = 1; //timer2 is turned on
    while (IFS0bits.T2IF == 0); //remains in the while loop until interrupt flag is raised
    T2CONbits.TON = 0; //timer2 is turned off


    //turn on timer2???
}

void getTimeString(unsigned minutes, unsigned seconds, unsigned FF) { //prints out Time string to LCD

    //variable declaration and initialization
    unsigned char temp = 0;
    unsigned int temp1 = 0;
    unsigned int temp2 = 0;
    unsigned int temp3 = 0;
    unsigned int temp4 = 0;
    unsigned int temp5 = 0;
    unsigned int temp6 = 0;

    if (minutes > 10) { //if minutes are greater than 10 calculate most and least significant digit
        temp1 = minutes / 10;
        temp2 = minutes % 10;
    }
    else if(minutes<10) { //if minutes are less than 10 store the value completely in temp 2, and put the most signficiant bit as 0
        temp1 = 0;
        temp2 = minutes;
    }

    else //if minutes == 10 set the bits to 10
    {
        temp1 = 1;
        temp2 = 0;
    }

    if (seconds > 10) { //if seconds are greater than 10 determine the most and least signficiant bit of "seconds"
        temp3 = seconds / 10;
        temp4 = seconds % 10;
    }
    else if(seconds<10){ //if seconds are less than 10 store the value completely in temp4
        temp3 = 0;
        temp4 = seconds;
    }

    else //if seconds are exactly 10 store it in the following two bits.
    {
        temp3 = 1;
        temp4 = 0;
    }

    if (FF > 10) { //if FF is greater than 10 determine the most and least signficiant bits and store them
        temp5 = FF / 10;
        temp6 = FF % 10;
    }
    else if(FF<10){ //if FF is less than 10 store the complete value in the least signficant bit
        temp5 = 0;
        temp6 = FF;
    }

    else //if FF == 10 set the bits directly to 10
    {
        temp5 = 1;
        temp6 = 0;
    }
  //temp is used as temporary storage character
  //it is constantly being reset to the values determined above in order to print to the LCD
  // MM:SS:FF, in that order
    temp = (char) temp1 + '0'; //'0' must be used in order to successfully typecast temp1 as a char
    printCharLCD(temp);
    temp = (char) temp2  + '0';
    printCharLCD(temp);
    printCharLCD(':');
    temp = (char) temp3  + '0';
    printCharLCD(temp);
    temp = (char) temp4  + '0';
    printCharLCD(temp);
    printCharLCD(':');
    temp = (char) temp5  + '0';
    printCharLCD(temp);
    temp = (char) temp6  + '0';
    printCharLCD(temp);

}



