/*
 * File:   ET420 Assignment 9
 * Author: Matt Alanskas
 *
 * Created on: 21 April 2021
 * Modified: 24 April 2021
 * 
 * Compiler: XC8 
 * Platform: PIC16F1823
 *
 * Description - Design and build a system 
 *               that will display the time 
 *               and temperature.
 * 
 * PORT A:    
 * RA0 (pin 13) not used
 * RA1 (pin 12) not used
 * RA2 (pin 11) not used
 * RA3 (pin 4) temp sensor
 * RA4 (pin 3) not used
 * RA5 (pin 2) not used
 * 
 * PORT C:
 * RC0 (pin 10) not used
 * RC1 (pin 9) not used
 * RC2 (pin 8) not used
 * RC3 (pin 7) not used
 * RC4 (pin 6) not used
 * RC5 (pin 5) not used
 */

// PIC16F1823 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = ECH       // Oscillator Selection (ECH, External Clock, High Power Mode (4-32 MHz): device clock supplied to CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

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

#define _XTAL_FREQ 8000000

char temp;
char second = 0;
char minute = 0;
char hour = 12;

void __interrupt() isr(void);   //prototype for interrupt subroutine

void main(void) 
    {
    
     // Port A
    PORTA = 0x00;               // clear port a
    LATA = 0x00;                // clear port a
    TRISA = 0x3e;               // RA0 output - rest all set as inputs
    ANSELA = 0x04;              // set RA2 to analog input
    WPUA = 0x0a;                // pull ups enabled on RA1, RA3
    
    // Port C 
    PORTC = 0x00;               // clear port c
    LATC = 0x00;                // clear port c
    ANSELC = 0x00;              // all digital
    TRISC = 0xff;               // all set as outputs
  
    OSCCON = 0x78;              // setting oscillator to 16MHz
    
    lcd_Initialize
   
    OPTION_REGbits.nWPUEN = 0;  // enabling pull-ups
        
    ADCON0 = 0x09;              // enabling a/d 
    ADCON1 = 0x93;              // right justify / 8 / fvr
    FVRCON = 0x83;              // setting fvr to 4.096V
   
    
}