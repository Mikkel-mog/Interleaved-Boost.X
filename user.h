
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */


#ifndef USER_H
#define	USER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define Ikp 5
#define Iki 10000
#define Ikd 0.0001

#define Vkp 5
#define Vki 10000
#define Vkd 0.0001

#define Ts 0.0001

void Setup_PPS(void);

void Setup_IOs(void);

void Setup_PID(void);

char Control_loop(unsigned int *Vin, unsigned int *Vout, unsigned int *Iout, unsigned int *Iref);



#endif	/* XC_HEADER_TEMPLATE_H */

