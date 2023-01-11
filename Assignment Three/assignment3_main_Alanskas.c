/*
 * File: ET420 Assignment Three
 * Author: Matt Alanskas
 *
 * Created on:  14 February 2021
 * Modified on: 19 February 2021
 * 
 * Compiler: XC8 
 * Platform: PIC16F1823
 *
 * Description - Wire a 10k? pot to the supply for the chip with the tap wired 
 *               to an analog input so that you can input any voltage 0-4V to 
 *               the pin.  Create a C application using a 16F1823 to read the 
 *               input voltage.  Display the tenth of a volt on a seven-segment 
 *               display.  Also have an LED driven from the PIC that is 
 *               inversely proportional to the input (full on @ 0V, off @ 4V). 
 *               Use an LM 324 to buffer the DAC output.  Use the fixed voltage 
 *               reference (4.096V) for the ADC.
 * 
 * PORT A:                                                                 
 * RA0 (pin 13) - lm324 
 * RA1 (pin 12) - potentiometer 
 * RA2 (pin 11) - not used
 * RA3 (pin 4)  - not used
 * RA4 (pin 3)  - segment b
 * RA5 (pin 2)  - segment a
 * 
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

const char diePa [] = {0x00,0x20,0x00,0x00,0x20,0x10,0x10,0x00,0x00,0x00,0x30};     // port a number array
const char diePc [] = {0x02,0x1e,0x24,0x0c,0x18,0x08,0x00,0x1e,0x00,0x18,0x3e};     // port c number array

float volts;                // converted voltage: output

void sevenDisplay(char);    // function to display number on seven seg

void main(void)
{
    // Port A
    PORTA = 0x00;      // clears port A
    LATA = 0x00;       // clear latch A
    ANSELA = 0x02;     // set RA1 to analog input
    TRISA = 0x0b;      // RA2, RA4,RA5 set as outputs
    
    // Port C
    PORTC = 0x00;      // clear port C
    LATC = 0x00;       // clear latch C
    ANSELC = 0x00;     // select all digital outputs
    TRISC = 0x01;      // RC1,RC2,RC3,RC4,RC5 set as outputs
    
    ADCON0 = 0x05;     // select AN1 / ADC enabled 
    ADCON1 = 0x83;     // right justified / Fosc / 2 clock / FVR
    
    FVRCON = 0x83;     // enable FVR / temp indicator off / 4.096 Volts
    
    DACCON0 = 0xf0;    // on / pos reference / pin / Vdd
    
    while(1)
    {
     ADCON0bits.GO_nDONE = 1;       // turn on AD converter
     
     while(GO_nDONE);               // wait for conversion to finish
     
     volts = ADRES*4.0039e-3;       // calculate voltage
     
     sevenDisplay(((char)(10*volts)) %10);          // display tenth
     
     DACCON1 = (char)((1023 - ADRES)*30.303e-3);    // calculating analog out value
    }
    
}
    
    void sevenDisplay(char d)   // function to take character and send it to the display
    {
        LATA = diePa [d];       // writes the value to port a of display
        LATC = diePc [d];       // writes the value to port c of display
    }
    
