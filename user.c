
//#include <pic16f1615.h>
#include <xc.h>
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

char Control_loop(unsigned int Vin, unsigned int Vout, unsigned int Iout, unsigned int Iref)
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
    
    PID1K1H = (uint8_t)(IK1>>8);
    PID1K1L = (uint8_t)IK1;
    PID1K2H = (uint8_t)(IK2>>8);
    PID1K2L = (uint8_t)IK2;
    PID1K3H = (uint8_t)(IK3>>8);
    PID1K3L = (uint8_t)IK3;
   /* PID1K1 = IK1;
    PID1K2 = IK2;
    PID1K3 = IK3;*/
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

    PID1SET = Iref;  // current ref here 
    PID1IN = (Iout<<1);    // current feedback shifted by one bit to multiply with 2

    while(PID1CONbits.BUSY)NOP();   //wait for finished calculation

    IZ1U = PID1Z1U;
    IZ1H = PID1Z1H;
    IZ1L = PID1Z1L;
    IZ2U = PID1Z2U;
    IZ2H = PID1Z2H;
    IZ2L = PID1Z2L;
    
    //state of current controller is saved
    if(!PID1OUTU)
    {
        if(IOUTHH & 0xFC)
        {
            IOUTHH = 0x03;
            IOUTHL = 0xff;
        }
        else
        {
            IOUTHH = PID1OUTHH;
            IOUTHL = PID1OUTHL;
        }
        IOUTLH = PID1OUTLH;
        IOUTLL = PID1OUTLL;
    }
    else
    {
        IOUTHH = PID1OUTHH = 0;
        IOUTHL = PID1OUTHL = 0;
    }

    unsigned int _Vref = IOUTHL | (((uint16_t)IOUTHH)<<8);

   // PORTA |= 0b00000100;
    _Vref += Vin;  // add input voltage to reference 
    
    if(_Vref>1023)   // saturate Vref at 60V
    {
        _Vref = 1023;
    }
   // PORTA &= 0b11111011;
     /*
     * Input K parameters for voltage controller
     * then the previous state of voltage controller is loaded
    */
    PID1K1H = (uint8_t)(VK1>>8);
    PID1K1L = (uint8_t)VK1;
    PID1K2H = (uint8_t)(VK2>>8);
    PID1K2L = (uint8_t)VK2;
    PID1K3H = (uint8_t)(VK3>>8);
    PID1K3L = (uint8_t)VK3;
    /*PID1K1 = VK1;
    PID1K2 = VK2;
    PID1K3 = VK3; */
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
    PID1IN = Vout;     // input voltage feedback and starts conversion

    while(PID1CONbits.BUSY)NOP() ;  //wait for finished calculation
    
    
    VZ1U = PID1Z1U;
    VZ1H = PID1Z1H;
    VZ1L = PID1Z1L;
    VZ2U = PID1Z2U;
    VZ2H = PID1Z2H;
    VZ2L = PID1Z2L;
    
    //state of voltage controller is saved
    char Duty_cycle;
    if(!PID1OUTU)
    {
        if(PID1OUTHH)
        {
            VOUTHH = 0;
            VOUTHL = 0xff;
            VOUTLH = PID1OUTLH;
            VOUTLL = PID1OUTLL;
        }
        else
        {
            VOUTHL = PID1OUTHL;
            VOUTLH = PID1OUTLH;
            VOUTLL = PID1OUTLL; 
        }

        Duty_cycle = VOUTHL;
    }
    else
    {
        Duty_cycle = 0;
    }        
    
    /*static uint16_t test= 0;
    test++;*/
    /*if(test == 1000)
    {
        test = 0;
    }*/
    return Duty_cycle;
}


void LoadDutyValue(uint16_t dutyValue)
{
    dutyValue &= 0x03FF;   
    //Load duty cycle value
    if(CCP1CONbits.FMT)
    {
        dutyValue <<= 4;
        dutyValue <<= 2;
        CCPR1H = (uint8_t)dutyValue >> 8;
        CCPR1L = (uint8_t)dutyValue;
        
        CCPR2H = (uint8_t)dutyValue >> 8;
        CCPR2L = (uint8_t)dutyValue;
    }
    else
    {
        CCPR1H = (uint8_t)dutyValue >> 8;
        CCPR1L = (uint8_t)dutyValue;
        
        CCPR2H = (uint8_t)dutyValue >> 8;
        CCPR2L = (uint8_t)dutyValue;
    }
}

void Period8BitSet(uint8_t periodVal)
{
   T2PR = periodVal;
   T4PR = periodVal-3;
}

//low pass filter with cornor frequency of 3 hz
//alpha = 1/256
unsigned int Vin_LP_filter(unsigned int in)
{
    static unsigned long filter = 0;
    static unsigned int result = 0;
    filter = (filter - result) + in;
    result = (uint16_t)(filter>>8);
    return result;
}


//low pass filter with cornor frequency of 25 hz
//alpha = 8/256
uint16_t Current_LP_filter(uint16_t in)
{
    static uint32_t filter = 0;
    static uint16_t result = 0;  
   
 
    filter = filter + (in<<1);
    filter = filter - (result<<1); 
    result = (uint16_t)(filter>>8);

    return result;
}

uint16_t Vout_LP_filter(uint16_t in)
{
    static uint32_t filter = 0;
    static uint16_t result = 0;  
    filter = filter + (in<<4);
    filter = filter - (result<<4); 
    result = (uint16_t)(filter>>8);

    return result;
}

/*
 * responce in duty cycle based on input fluctuations. 
 * new version takes into account the change needed based on 
 * the DC duty cycle
 */
int FF_CON(uint16_t in, uint16_t  in_f, uint16_t _Vout)
{
    uint16_t a = 1708 - _Vout;
    
    a = multiply(11,a);  
    
    a = (a>>8);
    a = (a>>1);

    uint16_t _inm = multiply(a,in);
    _inm = (_inm>>4);
    _inm = (_inm>>2);
   
    uint16_t _infm = multiply(a,in_f);
    _infm = (_infm>>4);
    _infm = (_infm>>2);

    int8_t result = (int8_t)_inm - (int8_t)_infm; 

    return result;
}


uint16_t multiply(uint16_t multiplier, uint16_t num)
{
    PID1OUTU = 0;
    PID1OUTHH = 0;
    PID1OUTHL = 0;
    PID1OUTLL = 0;
    PID1OUTLH = 0;
    PID1K1H = (uint8_t)(multiplier>>8);
    PID1K1L = (uint8_t)multiplier;
    PID1SET = num;
    PID1IN = 0;
    while(PID1CONbits.BUSY)NOP() ;  //wait for finished calculation
 
    uint16_t out = (uint16_t)PID1OUTLL | (((uint16_t)PID1OUTLH)<<8); 

    return out;
}





