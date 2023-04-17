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
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT enabled)
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


unsigned int Vout = 0;
unsigned int Vout_temp = 0;
unsigned int Iout = 0;
unsigned int Iout_temp = 0;

unsigned int Iref = 0;
unsigned int Iref_temp = 0;
unsigned int Vin = 0;
unsigned int Vin_temp = 0;
char new_sample = 0;
uint16_t CDC = 0;
uint8_t PWM_period = 39;

void __interrupt() ISR(void)
{
    static char state = 0;
   // PORTA |= 0b00000100;
    if(PIR2bits.TMR6IF)
    {
      PORTA |= 0b00000100;

        PIR2bits.TMR6IF = 0; // set low timer6 interrupt flag
    //    T1CONbits.TMR1ON = 1;         
        ADCON0 &= 0b10000011;
        ADCON0 |= 0b00010000;
        TMR1 = T1Period; 
        Period8BitSet(PWM_period);

        LoadDutyValue(CDC);

        /*state = 0;
        Vout = Vout_temp;
        Vin = Vin_temp;
        Iout = (Iout_temp<<1);
        Iref = Iref_temp;*/
        
        Iref = 512;
        Iout = 255;
        Vin = 512;          
        Vout = 200;
        state = 0;
        new_sample = 1;      

    }
    
   // PIR1bits.TMR1IF = 0;
     /*if(PIR1bits.TMR1IF)
    {
        PIR1bits.TMR1IF = 0;
        T1CONbits.TMR1ON = 0;   
        TMR1 = T1Period; 
    }*/

    if(PIR1bits.ADIF)
    {     
        PIR1bits.ADIF = 0;

        if(state==0)
        {            
            Iout_temp = ADRES;
            ADCON0 &= 0b10000011;
            ADCON0 |= 0b00011000;
            TMR1 = T1Period;
        //    T1CONbits.TMR1ON = 1;  
            state++;
        }
        else if(state == 1)
        {
            Vout_temp = ADRES;
            ADCON0 &= 0b10000011;
            ADCON0 |= 0b00011000;
            TMR1 = T1Period;
         //   T1CONbits.TMR1ON = 1;   
            state++;
        }
        else if(state == 2)
        {
            Iref_temp = ADRES;
            ADCON0 &= 0b10000011;
            ADCON0 |= 0b00011000;
            TMR1 = T1Period;
         //   T1CONbits.TMR1ON = 1;   
            state++;
        }
        else if(state == 3)
        {
            Vin_temp = ADRES;   
        }
           
    }
    if(PIR2bits.C1IF)
    {
        PIR2bits.C1IF = 0;
        CDC = 0;
        // set duty cycle to zero here
        // this is OVP
    }
   // PORTA &= 0b11111011; 
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
    
    Setup_PID();

    ConfigureINTERRUPT();


    TRISA &= 0b11111011;
    PORTA &= 0b11111011;
    PORTA |= 0b00000100;

    
    while(1)
    {

        if(new_sample)
        {
            new_sample = 0;
            
            if(Vin > 290 && !CM1CON0bits.C1OUT)
            {
              static uint8_t freq_iterator = 0; 
              freq_iterator++;
              if(freq_iterator == 20) freq_iterator = 0;
 
              uint16_t Vin_filtered = Vin_LP_filter(Vin);  


              uint16_t Iout_filtered = Current_LP_filter(Iout);
              
              uint16_t Vout_filtered = Vout_LP_filter(Vout);

              uint8_t Duty_cycle = Control_loop(Vin_filtered, Vout_filtered, Iout_filtered, Iref);  


              int8_t delta_d = FF_CON(Vin, Vin_filtered, Vout);

              if((int16_t)Duty_cycle - delta_d < 0 || Duty_cycle == 0) Duty_cycle = 0; 
              else
              {
                  Duty_cycle = Duty_cycle - delta_d;
              }


              
              if(Duty_cycle > 204) Duty_cycle = 204;


              uint16_t temp =(uint16_t)Duty_cycle*(uint16_t)arr_i[freq_iterator];//multiply(arr_i[freq_iterator],(uint16_t)Duty_cycle);
             
              temp = (temp>>4);
              temp = (temp>>2);
              
              CDC = Duty_cycle - temp;  
              
              PWM_period = 63 - arr_i[freq_iterator];
             
            }
            else 
            {
                CDC = 0;
                Iref = 0;
                
                Control_loop(Vin, Vout, Iout, Iref);   
            }
            PORTA &= 0b11111011;
        }
        
    }
    return;
}
