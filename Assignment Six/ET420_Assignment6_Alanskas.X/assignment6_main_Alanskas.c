/*
 * File:   ET420 Assignment 6
 * Author: Matt Alanskas
 *
 * Created on: 14 March 2021
 * Modified: 24 March 2021
 * 
 * Compiler: XC8 
 * Platform: PIC16F1823
 *
 * Description - I2C Network ? Program your 16F1823 
 *               so that it can communicate with everyone 
 *               else?s microprocessor in the class.
 * 
 * PORT A:    
 * RA0 (pin 13) D7
 * RA1 (pin 12) D6
 * RA2 (pin 11) D5
 * RA3 (pin 4) Name select
 * RA4 (pin 3) Master/slave select
 * RA5 (pin 2) Send
 * 
 * PORT C:
 * RC0 (pin 10) I2C
 * RC1 (pin 9)  I2C
 * RC2 (pin 8)  D4
 * RC3 (pin 7)  Enable
 * RC4 (pin 6)  R_nW
 * RC5 (pin 5)  RS
 */

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT enabled while running and disabled while sleeping)
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
#pragma config PLLEN = OFF      // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "lcd.h"

void __interrupt() pb_interrupt(void);
void masterMode(void);
void slaveMode(void);
void wait(void); 

#define James    0x04
#define DNathan  0x08
#define Martin   0x10
#define Jacob    0x12
#define Aaron    0x14
#define Indio    0x18
#define Ryan     0xfa
#define Evan     0x22
#define RNathan  0x24
#define Preston  0x28
#define Kyle     0x40
#define Eric     0x42
#define Mike     0x44

char i = 0;                             // increment variables for counting
char r = 1;                             // push button variable
const char slaveM[] = "Slave Mode ";    // string for slave mode
const char masterM[] = "Master Mode";   // string for master mode
int mode = 1;                           // different modes
char data;                              // data variable
char count;                             // counter variable

// classmate addresses 
const char address[] = {James, DNathan, Martin, Jacob, Aaron, Indio, Ryan, Evan, 
                            RNathan, Preston, Kyle, Eric, Mike};

// names of classmate addresses
const char * names[] = {"James  ","DNathan","Martin ","Jacob  ","Aaron  ",
                        "Indio  ","Ryan   ","Evan   ","RNathan","Preston",
                        "Kyle   ","Eric   ","Mike   "};

char name[] = "Matt";           // my name


void main(void) {
    
     // Port A
    PORTA = 0x00;               // clear port a
    LATA = 0x00;                // clear port a
    TRISA = 0x38;               // RA3, RA4, RA5 pb inputs
    ANSELA = 0x00;              // all digital
    WPUA = 0x38;                // pull ups enabled on RA3, RA4, RA5
    
    // Port C 
    PORTC = 0x00;               // clear port c
    LATC = 0x00;                // clear port c
    ANSELC = 0x00;              // all digital
    TRISC = 0x03;               // port c all outputs
    
    nWPUEN = 0;                 // weak pull up resistors enabled 
    
    OSCCON = 0x68;              // oscillator set to 4MHz
    
    SSP1STAT = 0x80;            // set slave mode register
            
    IOCIE = 1;                  // interrupt on change enabled
    IOCAN3 = 1;                 // negative ede IOC enabled for RA3
    IOCAN4 = 1;                 // negative ede IOC enabled for RA4
    
    IOCAF3 = 0;                 // reset IOC flag on RA3
    IOCAF4 = 0;                 // reset IOC flag on RA4
    
    ei();                       // interrupts enabled
    
    lcd_Initialize();           // Initialize LCD
    lcdDisplay(1,0,0);          // set cursor and blink off
    
    while(1)                    // begin while loop
     {
        
        if(mode == 1)           // code for slave mode
        {
            count = 0;          // counter variable
            slaveMode();        // call slave mode function
            goTo(4,0);          // starting position for text
            stringToLCD(slaveM); // print slave mode to lcd
            goTo(4,1);          // starting position for text
            SSP1IF = 0;         // clear interrupt flag
            

    while (mode == 1)           // code to stay in slave mode
    {        
        if(count > 3)           // check increment counter if greater than 3 - set zero
        {
            count = 0;          // reset count
            goTo(4,1);          // formatting output
        }

        if(SSP1IF)              // code to check if packet received
        {
          if(count < 4)
          {
          SSP1IF = 0;           // clear interrupt flag
          data = SSP1BUF;       // dSSP1BUF read address
          SSP1CON1bits.CKP = 1; // set clock bit 
          while(!SSP1IF);       // wait for IF to be set
          SSP1IF = 0;           // clear interrupt flag
          data = SSP1BUF;       // d=SSP1BUF - clears data buffer
          writeLCD(data,1);     // write data to lcd
          count++;              // increment counter
          SSP1CON1bits.CKP = 1; // set clock bit
          }
        }
    }
}
            
        else if(mode == 0)      // code for master mode
        {                        
            count = 0;              // counter variable
            masterMode();           // call slave mode function
            goTo(4,0);              // staring position for text
            stringToLCD(masterM);   // print master mode to lcd
            SSP1IF = 0;             // clear interrupt flag
            int j = 1;              // repeat variable j=1
            
     while(mode == 0)       // code to stay in master mode
        {        
           if(i != j)       // check increment counter if greater than 3 - set zero
        {
            if(i > 13)      // reset i after going through all
            {
                i=0;        // reset i after going through all
            }
            
            goTo(7,1);                  // formatting output
            stringToLCD(names[i]);      // display the names
            j=1;
            
        }
           if(RA5 == 0)
            {
             __delay_ms(300);           // delay - 300ms - switch
             
             for(int m = 0; m < 4; m++)
             {
                 wait();
                SSP1CON2bits.SEN = 1;   // set SEN bit
                
                wait();                 // wait for module
                SSP1BUF = address[i];   // SSP1BUF = address
                wait();                 // wait for module
                
                SSP1BUF = name[m];      // SSP1BUF = address
                SSP1CON2bits.PEN = 1;   // set pen bit
             }
                wait();                 // wait for module
            }
            }
        }
        }
    return;
    
    }
 
    void wait(void)          // code for function to make sure module is ready
      {
        while(SSP1CON2 & 0x1f || SSPSTAT & 0x04); // wait function
      }
    
    void slaveMode(void)     // function to put system in slave mode
      {
        cLCD();                 // clear LCD
        SSP1CON1 = 0x36;        // set SSP1 status control register one to slave mode
        SSP1CON2 = 0x01;        // set SSP1 status control register two to slave mode
        SSP1CON3 = 0x18;        // set SSP1 status control register three to slave mode
        SSP1ADD = Matt;         // my name
        
      }

    void masterMode(void)    // function to put system in master mode
      {
        SSP1CON1 = 0x28;        // set SSP1 status control register one to master mode
        SSP1CON2 = 0x00;        // set SSP1 status control register two to master mode
        SSP1CON3 = 0x00;        // set SSP1 status control register three to master mode
        SSP1ADD = 0x09;         // set clock to 4Mz - fclock to 100Mhz
      }

    void pb_interrupt(void)  // interrupt sub routine to handle button pushes
      {
        // push button one
        if (IOCAF3)             // if pushbutton one is engaged
        {
            __delay_ms(300);
            if(r % 2 == 1)
            {
                mode = 0;       // change to master mode when pb is engaged in slave mode
            }
            
            if(r % 2 == 0)
            {
                mode = 1;       // change to slave mode when pb is engaged in master mode
            }
            r += 1;             // increasing counter variable r
            IOCAF3 = 0;         // reset interrupt on change flag
        }
        
        // push button two
        if (IOCAF4)             // if pushbutton two is engaged
        {
           __delay_ms(300);     // delay
           i++;
           IOCAF4 = 0;          // reset interrupt on change flag   
        }
        
      }