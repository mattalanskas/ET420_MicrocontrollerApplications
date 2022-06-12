/*
 * File: ET420 Assignment Two
 * Author: Matt Alanskas
 *
 * Created on:  05 February 2021
 * Modified on: 09 February 2021 
 * 
 * Compiler: XC8 
 * Platform: PIC16F1823
 *
 * Description - Display the two values on one 7 segment display ? 
 *               alternate every two seconds.  Do this by using timer1
 *               driven by the clock ? Use an overflow interrupt to trigger
 *               the switch between values.
 * 
 * PORT A:                                                                 
 * RA0 (pin 13) - not used
 * RA1 (pin 12) - not used
 * RA2 (pin 11) - push button
 * RA3 (pin 4)  - not used
 * RA4 (pin 3)  - segment b
 * RA5 (pin 2)  - segment a
 * t4
 * PORT C:
 * RC0 (pin 10) - not used 
 * RC1 (pin 9)  - segment g
 * RC2 (pin 8)  - segment f
 * RC3 (pin 7)  - segment e
 * RC4 (pin 6)  - segment d
 * RC5 (pin 5)  - segment c
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

#include <xc.h>

#define _XTAL_FREQ 1000000      // defines 1MHz oscillator frequency 

const char diePa [] = {0x00,0x20,0x00,0x00,0x20,0x10,0x10,0x00,0x00,0x00,0x30};     // array for die 1 numbers
const char diePc [] = {0x02,0x1e,0x24,0x0c,0x18,0x08,0x00,0x1e,0x00,0x18,0x3e};     // array for die 2 numbers

void __interrupt() isr(void);       // prototype for interrupt function

void seg_display(char, char, char); // prototype for seven seg display function

volatile char die1 = 6;     // die 1 values
volatile char die2 = 6;     // die 2 values
volatile char counter = 0;  // count variable

void main(void)
{
    // Port A
    PORTA = 0x00;      //clears port A
    LATA = 0x00;       // clear latch A
    ANSELA = 0x00;     // selects all digital outputs
    TRISA = 0x0f;      // RA4,RA5 set as outputs
    WPUA = 0x04;       // push button
    
    // Port C
    PORTC = 0x00;      // clear port C
    LATC = 0x00;       // clear latch C
    ANSELC = 0x00;     // select all digital outputs
    TRISC = 0x01;      // RC1,RC2,RC3,RC4,RC5 set as outputs
    WPUC = 0x00;       // clear pull up
    
    nWPUEN = 0;        // enable pull-up resistors
    
    OSCCON = 0x58;     // clock set to 1Mhz 
    T1CON = 0x31;      // set timer prescale
    
    IOCIE = 1;         // enable interrupt on change  
    IOCAN2 = 1;        // enable interrupt on change of negative edge register
    IOCAF2 = 0;        // disables interrupt on change port a flag
    
    TMR1GE = 0;        // turns off timer 1 gate enable bit
    TMR1IE = 1;        // enable overflow for timer 1           
    TMR1IF = 0;        // turns off timer 1 flag
    TMR1ON = 1;        // turns on timer 1
            
    PEIE = 1;          // enable peripheral interrupt
    GIE = 1;           // enable global interrupt 
    
    while(1)
    {
        if(RA2 == 0)
        {
            LATA = diePa[8];   // latching port a to display die one values
            LATC = diePc[8];   // latching port c to display die two values
        }
        
        seg_display(die1, die2, counter);   // call seg_display function for die and counter variable
    }
    
    return;
}

void seg_display(char die1, char die2, char counter)
{
    if((counter % 2) == 1)
    {
        LATA = diePa[die1];     // latching ports to assign numbers to die 1
        LATC = diePc[die1];       
    }
        
    else
    {
        LATA = diePa[die2];     // latching ports to assign numbers to die 2
        LATC = diePc[die2];
    }
}

void isr (void)
{
    if(IOCAF2 == 1)
    {
        TMR1ON = 0;        // turn off timer 1 to set values
        
        die1 = TMR1L %5;   // first die is timer 1 mod 5
        die1 += 1;         // adding one to first die value
        
        die2 = TMR1L >> 2; // shifting the reading over two positions right 
        die2 = TMR1L %5;   // second die is timer 1 mod 5 shifted two positions right
        die2 += 1;         // adding one to second die value
        
        TMR1ON = 1;        // turn timer 1 back on
        IOCAF2 = 0;        // resetting the timer flag
    }
    
    if(TMR1IF)
    {
        TMR1ON = 0;     // turn off timer 1 to set values
        TMR1 = 0x85ee;  // setting timer one to 34,286
        counter++;      // counting variable
                
        TMR1ON = 1;     // turn timer 1 back on
        TMR1IF = 0;     // resetting the timer flag
    }
}
