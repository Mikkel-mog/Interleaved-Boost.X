

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
    CM2CON0 = 0x96; // enable output with inverted polarity + high speed and hysterasis 
    CM2CON1 = 0x23; // set FVR as + and C2in3- as negativ signal
    CM1CON0 = 0x94; // enable output with inverted output polarity + high speed
    CM1CON1 = 0x72; // set FVR as + and C1in2- as negative  + interrupt no falling edge 
    PIE2 = 0x20; // enables interrupt on comparator 1 edge
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