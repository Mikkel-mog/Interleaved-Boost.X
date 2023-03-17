
#include <pic16f1615.h>

#include "user.h"

void Setup_IOs(void)
{
    ANSELA = 0b00010100; // set RA4 and RA2 as analog
    TRISA = 0b00010000;
    ANSELC = 0x0F; // set RC0, RC1, RC2 and RC3 as analog 
    TRISC = 0x0F;
}

void Setup_PPS(void)
{
    RC5PPS = 0x00; // set to one pwm output
    RA5PPS = 0x00; // set to the other pwm output
    RC4PPS = 0x02; // RC4 is set to comparator 2 output 
}

void Setup_PID(void)
{
    PID1CONbits.MODE0 = 1;
    PID1CONbits.MODE1 = 0;
    PID1CONbits.MODE2 = 1;
    PID1CONbits.EN = 1;
}


unsigned int IK1 = (Ikp + Ts*Iki + Ikd/Ts);
unsigned int IK2 = -(Ikp + 2*Ikd / Ts);
unsigned int IK3 = (Ikd/Ts);

unsigned int VK1 = (Vkp + Ts*Vki + Vkd/Ts);
unsigned int VK2 = -(Vkp + 2*Vkd / Ts);
unsigned int VK3 = (Vkd/Ts);

char Control_loop(unsigned int *Vin, unsigned int *Vout, unsigned int *Iout, unsigned int *Iref)
{
    
    static char IOUTU = 0;
    static char IOUTHH = 0;
    static char IOUTHL = 0;
    static char IOUTLH = 0;
    static char IOUTLL = 0;
    static char IZ1U = 0;
    static char IZ1H = 0;
    static char IZ1L = 0;
    static char IZ2U = 0;
    static char IZ2H = 0;
    static char IZ2L = 0;

    static char VOUTU = 0;
    static char VOUTHH = 0;
    static char VOUTHL = 0;
    static char VOUTLH = 0;
    static char VOUTLL = 0;
    static char VZ1U = 0;
    static char VZ1H = 0;
    static char VZ1L = 0;
    static char VZ2U = 0;
    static char VZ2H = 0;
    static char VZ2L = 0;
    
    /*
     * Input K parameters for current controller
     * then the previous state of current controller is loaded
    */
    PID1K1 = IK1;
    PID1K2 = IK2;
    PID1K3 = IK3;
    PID1OUTU = IOUTU;
    PID1OUTHH = IOUTHH;
    PID1OUTHL = IOUTHL;
    PID1OUTLH = IOUTLH;
    PID1OUTLL = IOUTLL;
    PID1Z1U = IZ1U;
    PID1Z1H = IZ1H;
    PID1Z1L = IZ1L;
    PID1Z2U = IZ2U;
    PID1Z2H = IZ2H;
    PID1Z2L = IZ2L;

    PID1SET = *Iref;  // current ref here 
    PID1IN = (*Iout<<1);    // current feedback shifted by one bit to multiply with 2

    while(PID1CONbits.BUSY)NOP();   //wait for finished calculation

    //state of current controller is saved
    IOUTU = PID1OUTU;
    IOUTHH = PID1OUTHH;
    IOUTHL = PID1OUTHL;
    IOUTLH = PID1OUTLH;
    IOUTLL = PID1OUTLL;
    IZ1U = PID1Z1U;
    IZ1H = PID1Z1H;
    IZ1L = PID1Z1L;
    IZ2U = PID1Z2U;
    IZ2H = PID1Z2H;
    IZ2L = PID1Z2L;

    unsigned int Vref = IOUTHL | (IOUTHH*256);

    if(PID1OUTU)  // check for negativ value
    {
        Vref = *Vin;
        IOUTU = 0;
        IOUTHH = 0;
        IOUTHL = 0;
        IOUTLH = 0;
        IOUTLL = 0;
    }
    Vref += *Vin;  // add input voltage to reference 
    
    if(Vref>1024)   // saturate Vref at 60V
    {
        Vref = 1023;
        IOUTLL = (char)(Vref - *Vin);
        IOUTLH = (char)((Vref - *Vin)>>8);
    }
    
     /*
     * Input K parameters for voltage controller
     * then the previous state of voltage controller is loaded
    */
    
    PID1K1 = VK1;
    PID1K2 = VK2;
    PID1K3 = VK3; 
    PID1OUTU = VOUTU;
    PID1OUTHH = VOUTHH;
    PID1OUTHL = VOUTHL;
    PID1OUTLH = VOUTLH;
    PID1OUTLL = VOUTLL;
    PID1Z1U = VZ1U;
    PID1Z1H = VZ1H;
    PID1Z1L = VZ1L;
    PID1Z2U = VZ2U;
    PID1Z2H = VZ2H;
    PID1Z2L = VZ2L;

    PID1SET = Vref;     // input voltage reference
    PID1IN = *Vout;     // input voltage feedback and starts conversion

    while(PID1CONbits.BUSY)NOP() ;  //wait for finished calculation

    
    //state of voltage controller is saved
    VOUTU = PID1OUTU;
    VOUTHH = PID1OUTHH;
    VOUTHL = PID1OUTHL;
    VOUTLH = PID1OUTLH;
    VOUTLL = PID1OUTLL;
    VZ1U = PID1Z1U;
    VZ1H = PID1Z1H;
    VZ1L = PID1Z1L;
    VZ2U = PID1Z2U;
    VZ2H = PID1Z2H;
    VZ2L = PID1Z2L;


    char Duty_cycle = VOUTHL; 
    if(PID1OUTU) // check for negativ value
    {
        Duty_cycle = 0;
        VOUTU = 0;
        VOUTHH = 0;
        VOUTHL = 0;
        VOUTLH = 0;
        VOUTLL = 0;
    }
    else if(Duty_cycle > 120)    // sets upper saturation limit to 75%
    {
        Duty_cycle = 120; 
        VOUTHL = 0xA0;
        VOUTHH = 0x00;
    }
    return Duty_cycle;
}


