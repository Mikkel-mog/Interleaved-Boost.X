
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */


#ifndef USER_H
#define	USER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define Ikp 50
#define Iki 100
#define Ikd 0.1

#define Vkp 50
#define Vki 1000
#define Vkd 0.01

#define Ts 0.0002

uint8_t arr_i[] = {30, 12, 14, 15, 26, 28, 19, 29, 16, 18, 24, 22, 11, 31, 23, 13, 20, 21, 25, 17, 27};
uint8_t base_pr = 63;

void Setup_PPS(void);

void Setup_IOs(void);

void Setup_PID(void);

char Control_loop(unsigned int *Vin, unsigned int *Vout, unsigned int *Vref, unsigned int *Iout, unsigned int *Iref);

void LoadDutyValue(uint16_t dutyValue);

void Period8BitSet(uint8_t periodVal);

unsigned int LP_filter(unsigned int in);

int FF_CON(uint16_t in, uint16_t  in_f, uint16_t _Vout);
#endif	/* XC_HEADER_TEMPLATE_H */

