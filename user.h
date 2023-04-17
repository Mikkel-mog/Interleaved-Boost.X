
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */


#ifndef USER_H
#define	USER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define devider 65536
// IK values should all be multiplied with 2^16
#define Ikp 0.1 *devider
#define Iki 2.4 *devider
#define Ikd 0.00001*devider

// VK values should all be multiplied with 2^16
#define Vkp 0.01 * devider
#define Vki 136.45 *devider
#define Vkd 0.000004 *devider

#define Ts 0.00025

uint8_t arr_i[] = {30, 12, 14, 15, 26, 28, 19, 29, 16, 18, 24, 22, 11, 31, 23, 13, 20, 21, 25, 17, 27};
uint8_t base_pr = 63;

void Setup_PPS(void);

void Setup_IOs(void);

void Setup_PID(void);

char Control_loop(unsigned int Vin, unsigned int Vout, unsigned int Iout, unsigned int Iref);

void LoadDutyValue(uint16_t dutyValue);

void Period8BitSet(uint8_t periodVal);

unsigned int Vin_LP_filter(unsigned int in);
uint16_t Current_LP_filter(uint16_t in);
uint16_t Vout_LP_filter(uint16_t in);
uint16_t multiply(uint16_t multiplier, uint16_t num);

int FF_CON(uint16_t in, uint16_t  in_f, uint16_t _Vout);
#endif	/* XC_HEADER_TEMPLATE_H */

