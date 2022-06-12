/*
 * File: ET420 Assignment One
 * Author: Matt Alanskas
 *
 * Created on:  28 January 2021
 * Modified on: 2 February 2021 
 * 
 * Compiler: XC8 
 * Platform: PIC16F1823
 *
 * Description - This program should strobe the outside LEDs in a seven segment 
 *               display to give the illusion of a rotating circle. Use a 
 *               pushbutton to control the direction of the rotation.
 *               Holding down a pushbutton should reverse the rotation order.
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

int main(int argc, char** argv)
{
    // Port A
    PORTA = 0x00;      //clears port A
    LATA = 0x00;       // clear latch A
    ANSELA = 0x00;     // selects all digital outputs
    TRISA = 0x0f;      // RA4,RA5 set as outputs
    WPUA = 0x04;       // push button
    
    // Port C
    PORTC = 0x00;      // clear port C
    LATC = 0x00;
    ANSELC = 0x00;     // select all digital outputs
    TRISC = 0x01;      // RC1,RC2,RC3,RC4,RC5 set as outputs
    WPUC = 0x00;       // clear pull up
    
   char diePa [] = {0x1f,0x2f,0x3f,0x3f,0x3f,0x3f};
   char diePc [] = {0x3f,0x3f,0x1f,0x2f,0x37,0x3b};
    
    nWPUEN = 0;        //enable pull-up resistors
    OSCCON = 0x58;     // clock set to 1Mhz 
    
    while (1)
    {
        if(RA2)
        {
            for(int i=0; i<6; i++)      // loop keeping the segments on/off to display the racetrack pattern
            {
                LATA = diePa[i];
                LATC = diePc[i];
                
                __delay_ms(100);        // delay tells the segments to only remain on for 100 ms
            }
        }
        
        else
        {
            for(int i=5; i>-1; i--)     // loop keeping the segments on/off to display the racetrack pattern
            {
                LATA = diePa[i];
                LATC = diePc[i];
                
                __delay_ms(100);        // delay tells the segments to only remain on for 100 ms
            }
        }
}
    return(0);
}
