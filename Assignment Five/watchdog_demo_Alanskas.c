/*
 * File:   ET420 Assignment 5
 * Author: Matt Alanskas
 *
 * Created on: 5 March 2021
 * Modified: 8 March 2021
 * 
 * Compiler: XC8 
 * Platform: PIC16F1823
 *
 * Description - A simple program to demonstrate the 
 *               use of the sleep mode & watch dog timer
 *               in an embedded microcontroller.
 * 
 * PORT A:    
 * RA0 (pin 13) pb input - reset watch dog timer
 * RA1 (pin 12) pb input - go to sleep
 * RA2 (pin 11) D4 - lcd data
 * RA3 (pin 4)  not used
 * RA4 (pin 3)  not used
 * RA5 (pin 2)  not used
 * 
 * PORT C:
 * RC0 (pin 10) D5 - lcd data
 * RC1 (pin 9)  D6 - lcd data
 * RC2 (pin 8)  D7 - lcd data
 * RC3 (pin 7)  Enable - lcd
 * RC4 (pin 6)  R/W - lcd
 * RC5 (pin 5)  RS - lcd
 */

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = NSLEEP    // Watchdog Timer Enable (WDT enabled while running and disabled while sleeping)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#include "lcd.h"

void __interrupt() isr(void);

const char startingMsg[] = "Starting";  // starting message for LCD screen
const char runningMsg[] = "Running ";   // running message for LCD screen 
const char sleepMsg[] = "Sleeping";     // sleeping message for LCD screen
const char numbers[] = "123456789";     // numbers 1-9 to be printed to the LCD screen

void main(void) {
    
     // Port A
    LATA = 0x00;                // clear latch a
    TRISA = 0x3b;               // only RA2 is an output
    ANSELA = 0x00;              // all digital
    WPUA = 0x03;                // pull up enabled on RA0 and RA1
    
    // Port C 
    LATC = 0x00;                // clear latch c
    ANSELC = 0x00;              // all digital
    TRISC = 0x00;               // Port C all outputs
    WPUC = 0x00;                // no pull ups enabled
    
    nWPUEN = 0;                 // weak pull up resistors enabled 
    
    OSCCON = 0x58;              // oscillator set to 1MHz
    
    WDTCON = 0x18;              // 0b0001 1000 watch dog timer int set to - 4s
    
    IOCAN = 0x03;               // negative ede IOC enabled for RA0 and RA1
    INTCONbits.IOCIE = 1;       // interrupt on change enabled
    ei();                       // interrupts enabled
    
    lcd_Initialize();           // Initialize LCD
    lcdDisplay(1,0,0);          // set cursor and blink off
    
    goTo(4,0);                  // go to first line position 4
    stringToLCD(startingMsg);   // write starting to LCD
    __delay_ms(1000);           // one second delay
    
    while(1)
    {
        goTo(4,0);                          // go to first line position 4
        stringToLCD(runningMsg);            // write running to LCD
        
        for (unsigned char x=0;x<9;x++)     // write numbers 1-9 on second line
        {
            goTo(2,1);                      // go to third space second line
            writeLCD(numbers[x],1);         // write the numbers to the second line
            __delay_ms(1000);               // one second delay
            cLeft;                          // cursor left
        }
    }
    return;
}

void isr(void)
{
    if(IOCAF0)  // watch dog timer reset button pressed
    {
        CLRWDT();                   // clear WDT
        goTo(4,0);                  // go to first line position 4
        stringToLCD(runningMsg);    // write running to LCD
        IOCAF0 = 0;                 // reset interrupt flag
    }
    
    if(IOCAF1)  // go to sleep button pressed
    {
        IOCAF1 = 0;                 // reset interrupt flag
        goTo(4,0);                  // go to first line position 4
        stringToLCD(sleepMsg);      // write sleeping to LCD
        SLEEP();                    // go to sleep
        NOP();                      
    }
}