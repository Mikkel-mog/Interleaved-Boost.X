
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */


#ifndef USER_H
#define	USER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define Kp 1
#define Ki 0
#define Kd 0
#define Ts 0.0001

void Setup_PPS(void);

void Setup_IOs(void);



float PIDC(float);



#endif	/* XC_HEADER_TEMPLATE_H */

