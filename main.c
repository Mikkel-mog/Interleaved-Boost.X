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



void __interrupt() ISR(void)
{
    static ADC_set = 0; // use this to control what input to measure 
    if(TMR6IF_bit)
    {
        TMR6IF_bit = 0; // set low timer6 interrupt flag
        // set analog mux
        TMR1 = T1Period; // sets period for timer1
        TMR1ON_bit = 1; // enable clock to timer1
    }
    if(TMR1IF_bit)
    {
        TMR1IF_bit = 0; // set low TMR1 interrupt flag
        TMR1ON_bit = 0; // disable clock to timer1
    }
    if(ADIF_bit)
    {
        ADIF_bit = 0; // set low finished ADC conversion flag 
        if(ADC_set) 
        {
            // set next channel here
            TMR1 = T1Period;
            TMR1ON_bit = 1;
        }
    }
    if(C1IF_bit)
    {
        C1IF_bit = 0; // set comparator 1 interrupt bit low 
        // OVP control here set duty cycle to 
        // zero and start timer3 to wait to try again 
        // reset set IRef to zero until timer3 overflow
    }
}


void main(void) {

    while(1)
    {
        
    }
    return;
}
