/* 
 * File:   lcd.h
 * Author: Matt Alanskas
 * Comments: LCD library for PIC
 * Revision history: 3 March 2021
 */

// This is a guard condition so that contents of this file are not included
// more than once. 

#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h>     // include processor files - each processor file is guarded.  

  // Define

#define _XTAL_FREQ 1000000

#define Enable LATC3
#define R_nW LATC4
#define RS LATC5
#define D4 LATA2
#define D5 LATC0
#define D6 LATC1
#define D7 LATC2

#define cRight writeLCD(0x14, 0)    // cursor right definition 
#define cLeft writeLCD(0x10, 0)     // cursor left definition 
#define sRight writeLCD(0x1c, 0)    // shift right definition
#define sLeft writeLCD(0x18, 0)     // shift left definition

  // Initialization 

void writebyteLCD(unsigned char);                       // writebyteLCD function prototype
void lcd_Initialize(void);                              // initialization function prototype
void writeLCD(unsigned char out, unsigned char rs);     // writeLCD function prototype
void clockE(void);                                      // clock function prototype
void goTo(unsigned char pos, unsigned char line);       // goTo position function prototype
void cLCD(void);                                        // clear lcd function prototype


    // Display

void lcdDisplay(unsigned char D, unsigned char C, unsigned char B); // display function prototype
void lcdHome(void);                 // home position function prototype
void stringToLCD(char message[]);   // function prototype for writing string

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

