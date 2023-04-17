

#include "system.h"
#include <xc.h>
//#include <pic16f1615.h>


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
     CM2CON0bits.C2POL = 1;
    // enable high speed
    CM2CON0bits.C2SP = 1;
    //enable hystersis
    CM2CON0bits.C2HYS = 1;
    //disable synchrouns mode
    CM2CON0bits.C2SYNC = 0;   
    //set + input to FVR
    CM2CON1bits.C2PCH0 = 0;
    CM2CON1bits.C2PCH1 = 1;
    //set - input to C2IN3-
    CM2CON1bits.C2NCH2 = 0;
    CM2CON1bits.C2NCH1 = 1;
    CM2CON1bits.C2NCH0 = 1;  
    //inverts output polarity of comparator
    CM1CON0bits.C1POL = 1;
    //enable high speed mode
    CM1CON0bits.C1SP = 1;  
    //enable positive edge interupt
    CM1CON1bits.C1INTP = 1;   
    //set + input to FVR
    CM1CON1bits.C1PCH0 = 0;
    CM1CON1bits.C1PCH1 = 1;   
    //enable hystersis
    CM1CON0bits.C1HYS = 1;
    //set - input to C1in1-
    CM1CON1bits.C1NCH2 = 0;
    CM1CON1bits.C1NCH1 = 1;
    CM1CON1bits.C1NCH0 = 0;
    
    //enable both comparators 
    CM1CON0bits.C1ON = 1;
    CM2CON0bits.C2ON = 1;
}

void ConfigureADC(void)
{
    ADCON0 = 0x01;
    ADCON1 = 0b10100011;//0xA3; // set clock to FOSC/32 giving a Tad of 1us
    ADCON2 = 0x20;//  Sets timer1 overflow as trigger for auto conversion
}

#define T1Period 65536-224 // set acquisition period to 224 * 31.25ns 

void ConfigureTIMER(void)
{   
    T1CON = 0x40;    // set Timer 1 to have Fosc as clock source   
    // T2CS FOSC/4; 
    T2CLKCON = 0x00;
    // T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; 
    T2HLT = 0x00;
    // T2RSEL T2IN; 
    T2RST = 0x00;
    // T2PR 39; 5us Period
    T2PR = 0x27;
    // TMR2 0; 
    T2TMR = 0x00;
    // Clearing IF flag.
    PIR1bits.TMR2IF = 0;
    // T2CKPS 1:1; T2OUTPS 1:1; TMR2ON on; 
    T2CON = 0x80;
    // T4CS FOSC/4; 
    T4CLKCON = 0x00;
    // T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC  Synchronized; 
    T4HLT = 0b00110010; //0x31
    // T4RSEL ccp1 out; 
    T4RST = 0b00000011; // 0x04
    // T4PR 39; 
    T4PR = 0x20;
    // TMR4 0; 
    T4TMR = 0x00;
    // Clearing IF flag.
    PIR2bits.TMR4IF = 0;
    // T4CKPS 1:1; T4OUTPS 1:1; TMR4ON on; 
    T4CON = 0x80;     
    // set sample rate to 4khz    
    T6CLKCON = 0x00;  // sets clock to FOSC/4 with a period of 125ns 
    T6CON = 0x40;     // set prescaler to 4 and no postscaler giving a period of 500ns 
    T6HLT = 0x00;     // Sets mode to software gate free running, reset automatically when TMR6 = PR6
    PR6 = 124;       //  PR6 should be set to the time between samples here it is 4khz (250us/2us -1)

    T6CON |= 0x80;
            
            
}

void ConfigurePWM(void)
{
    // MODE PWM; EN enabled; FMT right_aligned; 
	CCP1CON = 0x8F; 
    CCP2CON = 0x8F; 
	
	// RH 0; 
	CCPR1H = 0x00;    
    CCPR2H = 0x00; 
	
	// RL 79; 
	CCPR1L = 0x00;  // 50% duty cycle 
    CCPR2L = 0x00;  
	
	CCPTMRSbits.CCP1TSEL = 0x0; // Selecting Timer 2
    CCPTMRSbits.CCP2TSEL = 0x1; // Selecting Timer 4
}

void ConfigureINTERRUPT(void)
{
   //PIE1bits.TMR1IE = 1;
    T1CONbits.TMR1ON = 1;
    PIE1bits.ADIE = 1;
    PIE2bits.C1IE = 1;
    PIE2bits.TMR6IE = 1;
    
    GIE = 1;
    PEIE = 1;
}