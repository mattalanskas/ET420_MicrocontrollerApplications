/*
 * File:   ET420 Assignment 4
 * Author: Matt Alanskas
 *
 * Created on: 17 February 2021
 * Modified: 3 March 2021
 * 
 * Compiler: XC8 
 * Platform: PIC16F1823
 *
 * Description - Short Demo Program for LCD Library
 * 
 * PORT A:    
 * RA0 (pin 13) not used
 * RA1 (pin 12) not used
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
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
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
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "lcd.h"

#define _XTAL_FREQ 1000000

void main(void) 
{
    // Port A
    PORTA = 0x00;               // clear port a
    LATA = 0x00;                // clear latch a
    TRISA = 0x3b;               // only RA2 is an output
    ANSELA = 0x00;              // all digital
    
    // Port C
    PORTC = 0x00;               // clear port c
    LATC = 0x00;                // clear latch c
    ANSELC = 0x00;              // all digital
    TRISC = 0x00;               // Port C all outputs
    
    OSCCON = 0x58;              // oscillator set to 1MHz
     
    lcd_Initialize();           // initialize LCD
    
    int i;
    const char message[] = "LCD Library";   // defining message as "LCD Library" 
    const char by[] = "Ryan Morrissey";     // defining by as "Ryan Morrissey"
    const char scroll[] = "Shift";          // defining scroll as "Shift"
    const char cursor[]= "Csr";             // defining cursor as "Csr"
    const char dis[] = "Display";           // defining dis as "Display"
    
    stringToLCD(message);       // print "LCD Library"
    goTo(0,1);                  // move to second row
    stringToLCD(by);            // print "Ryan Morrissey"
   __delay_ms(2000);            // two second delay
   
   lcdHome();                   // moves cursor to home position
   __delay_ms(1000);            // one second delay

   for (i = 0; i<8; i++)        // for loop for right shift
   {
       sRight;                  // shift right eight positions
       __delay_ms(500);         // half second delay
   }
   
   for(i = 0; i<8; i++)         // for loop for left shift   
   {
       sLeft;                   // shift left eight positions
       __delay_ms(500);         // half second delay
   }
   
   cLCD();                      // clear lcd
   __delay_ms(1000);            // one second delay
    
   
   // Display demo
   
   lcdDisplay(0,0,0);
   __delay_ms(1000);
    lcdDisplay(1,1,1);
    __delay_ms(1000);
   lcdDisplay(1,0,1);
   __delay_ms(1000);
   lcdDisplay(1,1,0);
   __delay_ms(1000); 
   lcdDisplay(1,1,1);
   __delay_ms(1000);
   
   lcdHome();                    // moves cursor to home position           
   cLCD();                       // clear lcd
   stringToLCD(message);         // print "lcd library"
   goTo(0,1);                    // move to second row
   stringToLCD(cursor);          // print "csr"
   goTo(0,0);
   
   __delay_ms(1000);

   for (i = 0; i<8; i++)         // for loop for cursor right
   {
       cRight;                   // cursor right eight positions
       __delay_ms(500);          // half second delay
   }
   
   for (i = 0; i<8; i++)         // for loop for cursor left
   {
       cLeft;                    // cursor left eight positions         
       __delay_ms(500);          // half second delay
   }
     __delay_ms(3000);           // three second delay
     
  cLCD();                        // clear lcd
 
    return;
}