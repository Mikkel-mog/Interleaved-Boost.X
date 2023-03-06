
#include <pic16f1615.h>

#include "user.h"

void Setup_IOs(void)
{
    ANSELA = 0b00010100; // set RA4 and RA2 as analog
    TRISA = 0b00010100;
    ANSELC = 0x0F; // set RC0, RC1, RC2 and RC3 as analog 
    TRISC = 0x0F;
}

void Setup_PPS(void)
{
    RC5PPS = 0x00; // set to one pwm output
    RA5PPS = 0x00; // set to the other pwm output
    RC4PPS = 0x02; // RC4 is set to comparator 2 output 
}


float PIDC(float E)
{
    static float E1 = 0;
    static float acu = 0;
    acu += Ts/2  * (E+E1);
    acu = acu > 1 ? 1 : acu < 0 ? 0 : acu;
    //if(acu > 1) acu = 1;
    //if(acu < 0) acu = 0; 
    float dE = (E - E1)/Ts;
    
    float Result = E*Kp + acu*Ki + dE*Kd;
    
    if(Result < 0) Result = 0;
    if(Result > 1) Result = 1;
    E1 = E;
    return Result;
}


