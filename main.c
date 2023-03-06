/*
 * File:   main.c
 * Author: mikkel
 *
 * Created on 1. marts 2023, 14:54
 */


#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switch Over (Internal External Switch Over mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit cannot be cleared once it is set by software)
#pragma config ZCD = OFF        // Zero Cross Detect Disable Bit (ZCD disable.  ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PLLEN = ON       // PLL Enable Bit (4x PLL is always enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// CONFIG3
#pragma config WDTCPS = WDTCPS1F// WDT Period Select (Software Control (WDTPS))
#pragma config WDTE = ON        // Watchdog Timer Enable (WDT enabled)
#pragma config WDTCWS = WDTCWSSW// WDT Window Select (Software WDT window size control (WDTWS bits))
#pragma config WDTCCS = SWC     // WDT Input Clock Selector (Software control, controlled by WDTCS bits)



#include <xc.h>
#include "system.h"
#include "user.h"


#define T1Period 65536-224 // set acquisition period to 224 * 31.25ns 


unsigned char C1IFLAG = 0x20;
unsigned char TMR6IFLAG = 0x04;
unsigned char TMR1IFLAG = 0x01;
unsigned char ADCIFLAG = 0x40;
unsigned char TMR1ONC = 0x01;


unsigned int current_meas = 0;
unsigned int voltage_out_meas = 0;
unsigned int ref_meas = 0;
unsigned int voltage_in_meas = 0;



void __interrupt() ISR(void)
{
    static ADC_set = 0; // use this to control what input to measure 
     
    if(PIR2 && TMR6IFLAG)
    {
        PIR2 &= ~TMR6IFLAG; // set low timer6 interrupt flag
        ADCON0 &= 0x01;  //set mux to zero and go to off 
        ADCON0 |= 0x10; // sets AN4 as input to ADC
        TMR1 = T1Period; // sets period for timer1
        T1CON |= TMR1ONC; // enable clock to timer1
        ADC_set = 1;
    }
    
    if(PIR1 && TMR1IFLAG)
    {
        PIR1 &= ~TMR1IFLAG; // set low TMR1 interrupt flag
        T1CON &= ~TMR1ONC;  //disable clock to timer1
    }
    
    if(PIR1 && ADCIFLAG)
    {
        PIR1 &= ~ADCIFLAG; // set low finished ADC conversion flag 
        switch(ADC_set)
        {
            case 0 :
                current_meas = ADRES;
                ADCON0 &= 0x01;  //set mux to zero and go to off 
                ADCON0 &= 0x14;   // set AN5 as input 
                TMR1 = T1Period;
                T1CON |= TMR1ONC;
                ADC_set++;
                break;
            case 1:
                voltage_out_meas = ADRES;
                ADCON0 &= 0x01;  //set mux to zero and go to off 
                ADCON0 &= 0x08;   // set AN5 as input 
                TMR1 = T1Period;
                T1CON |= TMR1ONC;
                ADC_set++;
                break;
            case 2:
                ref_meas = ADRES;
                ADCON0 &= 0x01;  //set mux to zero and go to off 
                ADCON0 &= 0x0C;   // set AN5 as input 
                TMR1 = T1Period;
                T1CON |= TMR1ONC;
                ADC_set++;
                break;
            case 3:
                voltage_in_meas = ADRES;
                ADC_set++;
            default:
                break;
        }       
    }
    if(PIR2 && C1IFLAG)
    {
        PIR2 = ~C1IFLAG; // set comparator 1 interrupt bit low 
        // OVP control here set duty cycle to 
        // zero and start timer3 to wait to try again 
        // reset set IRef to zero until timer3 overflow
    }
}


void main(void) {

    ConfigureOscillator();
    ConfigureWDT();
    ConfigureFVR();
    ConfigureComparator();
    ConfigureADC();
    ConfigureTIMER();
    ConfigurePWM();
    Setup_IOs();
    Setup_PPS();
    ConfigureINTERRUPT();
    
    while(1)
    {
        
    }
    return;
}
