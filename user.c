
#include <pic16f1615.h>

#include "user.h"

void Setup_PPS(void)
{
    RC2PPS = 0x00;
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


