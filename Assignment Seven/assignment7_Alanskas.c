/*
 * File:   ET420 Assignment 7
 * Author: Matt Alanskas
 *
 * Created on: 29 March 2021
 * Modified: 1 April 2021
 * 
 * Compiler: XC8 
 * Platform: PIC16F1823
 *
 * Description - PWM module ? use the PWM module to vary 
 *               the brightness of an LED.
 * 
 * PORT A:    
 * RA0 (pin 13) not used
 * RA1 (pin 12) not used
 * RA2 (pin 11) input from potentiometer
 * RA3 (pin 4) not used
 * RA4 (pin 3) not used
 * RA5 (pin 2) not used
 * 
 * PORT C:
 * RC0 (pin 10) not used
 * RC1 (pin 9) not used
 * RC2 (pin 8) not used
 * RC3 (pin 7) not used
 * RC4 (pin 6) not used
 * RC5 (pin 5) output to drive LED
 */

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = ON        // Watchdog Timer Enable (WDT enabled while running and disabled while sleeping)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL enabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

int pwmDutyCycle;               // defining duty cycle to be read from converter

void main(void) {
    
     // Port A
    PORTA = 0x00;               // clear port a
    LATA = 0x00;                // clear port a
    TRISA = 0x3f;               // RA0-RA5 all set as inputs
    ANSELA = 0x17;              // set RA0-RA5 to analog inputs
    
    // Port C 
    PORTC = 0x00;               // clear port c
    LATC = 0x00;                // clear port c
    ANSELC = 0x00;              // all digital
    TRISC = 0x01;               // set RC1-RC5 as outputs
  
    OSCCON = 0x58;              // setting oscillator to 1MHz
   
    ADCON0 = 0x09;              // pot on RA2 - not in progress - AD conversion enabled
    ADCON1 = 0x00;              // left justified - Fosc/2 - Vref to AVdd
    
    PR2 = 0xff;                 // max value for pwm period
        
    CCP1CON = 0x0c;             // PWM mode - P1A, P1C active high - P1B, P1D active high
        
    TMR2ON = 1;                 // turn on timer two
    TMR2IF = 0;                 // reset timer two interrupt flag
    
    FVRCON = 0x83;              // enable FVR - 4.096V
    
    while(1)
    {             
        ADCON0bits.GO_nDONE = 1;         // turn on AD converter
        while(ADCON0bits.GO_nDONE == 0); // waiting for conversion to finish
        pwmDutyCycle = ADRESH;           // reading voltage results from AD converter
        CCPR1 = ADRESH;                  // setting the analog voltage to the results from the conversion            
    }
    
  return;
  
} 
