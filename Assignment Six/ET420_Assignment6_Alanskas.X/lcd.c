/*
 * File:   lcd.c
 * Author: Matt Alanskas
 *
 * Created on: February 17 2021
 * Modified: 3 March 2021
 * 
 */

#include <xc.h>
#include <string.h>
#include "lcd.h"

void writebyteLCD(unsigned char out)    // function to set data pins
{
    unsigned char mask = 0x80;              // = 1000 0000
    D7 = ((out & mask) == mask) ? 1:0;      // sets D7 to most significant bit
    mask = 0x40;                            // = 0100 0000
    D6 = ((out & mask) == mask) ? 1 : 0 ;   // sets D6 to most significant bit
    mask = 0x20;                            // = 0010 0000
    D5 = ((out & mask) == mask) ? 1 : 0 ;   // sets D5 to most significant bit 
    mask = 0x10;                            // = 0001 0000
    D4 = ((out & mask) == mask) ? 1 : 0 ;   // sets D4 to most significant bit
    
    return;
}

void clockE(void)          // enable clock for read / write
{
    Enable = 1;            // turn on clock
    __delay_us(10);        // 10us delay
    Enable = 0;            // turn off clock
    __delay_us(10);        // 10us delay
    
    return;
}
    
    void writeLCD(unsigned char out, unsigned char rs) // writing characters to the lcd
{
    RS = rs;
    writebyteLCD(out);     // call writebyteLCD function
    clockE();              // clock function
    __delay_us(200);       // 200us delay
    out = out<<4;
    writebyteLCD(out);     // call writebyteLCD function
    clockE();              // clock 
    
    return;
}

    void lcd_Initialize(void)
{
    
    __delay_ms(15);             // 15ms delay for the display to stabilize
    RS = 0;                     // prepare lcd instruction
    
    //*************************************************************************
    
    writebyteLCD(0x30);         // set pins for first instruction
    clockE();                   // call clock function
    
    //*************************************************************************
    
    __delay_ms(5);              // 5ms delay
    clockE();                   // call clock function
    
    //*************************************************************************
    
    __delay_us(200);            // 200us delay
    clockE();                   // call clock function
    
    //*************************************************************************
    
    __delay_us(200);            // 200us delay
    writebyteLCD(0x20);         // set data pins
    clockE();                   // call clock function
    
    //*************************************************************************
    
    __delay_us(200);            // 200us delay
    writeLCD(0x28,0);           // Function Set DL = 0 4 bit, N = 1 2 lines
    
    //*************************************************************************
    
    __delay_us(200);            // 200us delay
    writeLCD(0x0f,0);           // entry mode     
    
    
    //*************************************************************************
    
    __delay_us(200);            // 200us delay
    writeLCD(0x06,0);           // display, cursor, blinking on
    
    //*************************************************************************
    
     __delay_us(200);           // 200us delay
     writeLCD(0x01,0);          // clear lcd display 
     
    //*************************************************************************
   
     __delay_ms(5);             // 5ms delay
      writeLCD(0x02,0);         // Return Home
      
    //*************************************************************************
      
    __delay_ms(5);              // 5ms delay
    
    /* Initialization Complete */  
    
    return;
}


    void goTo(unsigned char pos, unsigned char line)
{
    if(pos > 39)
        return;
    pos += 0x80;          // add a one in most significant digit for command
    if(line == 1)
        pos += 0x40;      // if second line add 0100 0000 to get to next line
    writeLCD(pos,0);
    __delay_us(200);
    
    return;
}

    void cLCD(void)       // clear LCD display function
{
      writeLCD(0x01,0);   
      __delay_ms(4);      // 4ms delay
      
      return;
}
    
    void lcdDisplay (unsigned char D, unsigned char C, unsigned char B) // lcd Display function
{
        if(D==1)
            B |= 0x04;
        if(C==1)
            B |= 0x02;
        
            B |= 0x08;
        
        writeLCD(B,0);      // call writeLCD function
        __delay_us(200);    // 200us delay
        
        return;
}
    
    void lcdHome(void)      // moving back to the home point
{
        writeLCD(0x02,0);   // sends cursor back to the home position
        clockE();           // call clock function
        __delay_ms(4);      // 4ms delay
        
        return;
}
    
    void stringToLCD(char message[])    // writing a string to the LCD
{
    unsigned int i = strlen(message);
    for(char x = 0; x<i; x++)           // for loop to ensure each character of string is included
    {    
        writeLCD(message[x],1);         // using writeLCD function to write each character of the desired string
    }
    return;
}