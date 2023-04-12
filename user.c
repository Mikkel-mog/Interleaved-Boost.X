
#include <pic16f1615.h>

#include "user.h"

void Setup_IOs(void)
{
       
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 0;
    
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;        
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    
    
    ANSELAbits.ANSA2 = 1;
    ANSELAbits.ANSA4 = 1;
    
    ANSELCbits.ANSC0 = 1;
    ANSELCbits.ANSC1 = 1;
    ANSELCbits.ANSC2 = 1;
    ANSELCbits.ANSC3 = 1;

}

void Setup_PPS(void)
{
    RA5PPS = 0x0C;   //RA0 ->CCP1   
    RC5PPS = 0x0D;   //RA2->CCP2
    RC4PPS = 0x02; // RC4 is set to comparator 2 output 
}

void Setup_PID(void)
{
    PID1CONbits.MODE0 = 1;
    PID1CONbits.MODE1 = 0;
    PID1CONbits.MODE2 = 1;
    PID1CONbits.EN = 1;
}


int IK1 = (Ikp + Ts*Iki + Ikd/Ts);
int IK2 = -(Ikp + 2*Ikd / Ts);
int IK3 = (Ikd/Ts);

int VK1 = (Vkp + Ts*Vki + Vkd/Ts);
int VK2 = -(Vkp + 2*Vkd / Ts);
int VK3 = (Vkd/Ts);

char Control_loop(unsigned int *Vin, unsigned int *Vout, unsigned int *Vref, unsigned int *Iout, unsigned int *Iref)
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

    PID1SET = (*Iref<<1);  // current ref here 
    PID1IN = *Iout;    // current feedback shifted by one bit to multiply with 2

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

    unsigned int _Vref = IOUTLH | (IOUTHL*256);

    if(PID1OUTU)  // check for negativ value
    {
        _Vref = *Vin;
        IOUTU = 0;
        IOUTHH = 0;
        IOUTHL = 0;
        IOUTLH = 0;
        IOUTLL = 0;
    }
    _Vref += *Vin;  // add input voltage to reference 
    
    if(_Vref>1024)   // saturate Vref at 60V
    {
        _Vref = 1023;
        IOUTLL = (char)(_Vref - *Vin);
        IOUTLH = (char)((_Vref - *Vin)>>8);
    }
    
    *Vref = _Vref;
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

    PID1SET = _Vref;     // input voltage reference
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


    char Duty_cycle = VOUTLH; 
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


void LoadDutyValue(uint16_t dutyValue)
{
    dutyValue &= 0x03FF;
    
    // Load duty cycle value
    if(CCP1CONbits.FMT)
    {
        dutyValue <<= 6;
        CCPR1H = dutyValue >> 8;
        CCPR1L = dutyValue;
        
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue;
    }
    else
    {
        CCPR1H = dutyValue >> 8;
        CCPR1L = dutyValue;
        
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue;
    }
}

void Period8BitSet(uint8_t periodVal)
{
   T2PR = periodVal;
   T4PR = periodVal-3;
}

unsigned int LP_filter(unsigned int in)
{
    static unsigned long filter = 0;
    static unsigned int result = 0;
    filter = filter - result + in;
    result = (filter>>8);
    return result;
}


/*
 * responce in duty cycle based on input fluctuations. 
 * new version takes into account the change needed based on 
 * the DC duty cycle
 */
int FF_CON(uint16_t in, uint16_t  in_f, uint16_t _Vout)
{
    uint16_t a = (((1708 - _Vout)*11)>>9) ; 
    return (int)((in * a)>>6) - (int)((in_f * a)>>6);   
}
