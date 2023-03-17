

#include "system.h"
#include <xc.h>
#include <pic16f1615.h>


void ConfigureOscillator(void)
{
    OSCCON = 0xF0;   // set clock as 32Mhz 
}

void ConfigureWDT(void)
{
    
}

void ConfigureFVR(void)
{
    FVRCON = 0x8A; // set the FVR on, ADCREF to 2.048V and comparator ref to 2.048V
}

void ConfigureComparator(void)
{
    //inverts output polarity
    CM2CON0bits_t.C2POL = 1;
    // enable high speed
    CM2CON0bits_t.C2SP = 1;
    //enable hystersis
    CM2CON0bits_t.C2HYS = 1;
    //disable synchrouns mode
    CM2CON0bits_t.C2SYNC = 0;   
    //set + input to FVR
    CM2CON1bits_t.C2PCH0 = 0;
    CM2CON1bits_t.C2PCH1 = 1;
    //set - input to C2IN3-
    CM2CON1bits_t.C2NCH2 = 0;
    CM2CON1bits_t.C2NCH1 = 1;
    CM2CON1bits_t.C2NCH0 = 1;  
    //inverts output polarity of comparator
    CM1CON0bits_t.C1POL = 1;
    //enable high speed mode
    CM1CON0bits_t.C1SP = 1;  
    //enable positive edge interupt
    CM1CON1bits_t.C1INTP = 1;   
    //set + input to FVR
    CM1CON1bits_t.C1PCH0 = 0;
    CM1CON1bits_t.C1PCH1 = 1;   
    //set - input to C1in1-
    CM1CON1bits_t.C1NCH2 = 0;
    CM1CON1bits_t.C1NCH1 = 1;
    CM1CON1bits_t.C1NCH0 = 0;
    
    //enable both comparators 
    CM1CON0bits_t.C1ON = 1;
    CM2CON0bits_t.C2ON = 1;
    // enables interrupt on comparator 1 edge
    PIE2bits_t.C1IE = 1;
}

void ConfigureADC(void)
{
    
    ADCON0 = 0x01;
    ADCON1 = 0xA3; // set clock to FOSC/32 giving a Tad of 1us
    ADCON2 = 0x20;//  Sets timer1 overflow as trigger for auto conversion
    ADIE = 1;// interrupt on finished conversion 
}

#define T1Period 65536-224 // set acquisition period to 224 * 31.25ns 

void ConfigureTIMER(void)
{
    
    T1CON = 0x40;    // set Timer 1 to have Fosc as clock source
    PIE1 = 0x01;     //enables interrupt on timer1 overflow
    
    // set sample rate to 5khz
    T6CLKCON = 0x00;  // sets clock to FOSC/4 with a period of 125ns 
    T6CON = 0x40;     // set prescaler to 16 and no postscaler giving a period of 2us 
    T6HLT = 0x00;     // Sets mode to software gate free running, reset automatically when TMR6 = PR6
    PR6 = 100;       // PR6 should be set to the time between samples 
    PIE2 |= 0x04;   // enables interrupt on PR6-TMR6 match
    T6CON |= 0x80;
}

void ConfigurePWM(void)
{
    
}

void ConfigureINTERRUPT(void)
{
    GIE = 1;
    PEIE = 1;
}