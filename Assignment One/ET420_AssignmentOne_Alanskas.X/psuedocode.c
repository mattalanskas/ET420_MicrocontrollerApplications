Set all the congifuration bits

Include xc.h

Set frequency to 1000000   

unsigned short count = 0;

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
            for(int i=0; i<6; i++)
            {
                LATA = diePa[i];
                LATC = diePc[i];
                
                __delay_ms(100);
            }
        }
        
        else
        {
            for(int i=5; i>-1; i--)
            {
                LATA = diePa[i];
                LATC = diePc[i];
                
                __delay_ms(100);
            }
        }
}
    return(0);
}