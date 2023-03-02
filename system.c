

#include "system.h"
#include <xc.h>


void ConfigureOscillator(void)
{
    OSCCON = 0xF0;   
}

void ConfigureWDT(void)
{
    
}

void ConfigureFVR(void)
{
    FVRCON = 0x8F;
}

void ConfigureComparator(void)
{
    CM1CON1 = 0x72;
    CM1CON0 = 0x94;
}

void ConfigureADC(void)
{
    ADCON1 = 0x63;
    //ADCON2    can set a trigger source for automatic conversion
    // interrupt can be enabled on finished conversion
}


void ConfigureTIMER(void)
{
    PR4 = 79;
    T4CLKCON = 0x00;
    TMR4ON = 1;    
    T4CON = 0b10000000;
    TMR4IE = 1;
}

void ConfigurePWM(void)
{
    
}

void ConfigureINTERRUPT(void)
{
    
}