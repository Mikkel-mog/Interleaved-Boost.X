
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
#define Ikp 0.1 *0.5*devider
#define Iki 2.4 *0.5*devider
#define Ikd 0.00001*0.5*devider

// VK values should all be multiplied with 2^16
#define Vkp 0.01 * 60 / 1024 * devider
#define Vki 136.45*2 *60 / 1024 *devider
#define Vkd 0.000004*60 / 1024 *devider

#define Ts 0.00025

uint8_t arr_i[] = {30, 12, 14, 15, 26, 28, 19, 29, 16, 18, 24, 22, 11, 31, 23, 13, 20, 21, 25, 17, 27};

uint16_t unlinearity_correction[64] = {0,0,0,0,0,0,0,0,58	,51	,45	,41	,37	,33
        ,31	,28	,26	,24	,22	,20	,19	,18	,16	,15	,14	,13	,13	,12	,11	,10	,10	,9
        ,8	,8	,7	,7	,6	,6	,6	,5	,5	,4	,4	,4	,4	,3	,3	,3	,2	,2
        ,2	,2	,1	,1	,1	,1	,1	,0	,0	,0	,0	,0	,0	,0};
uint16_t unlinearity_correction_steep[32] = {2422	,1206	,801	,598	,476
        ,395	,337	,294	,260	,233	,211	,193	,177	,164
        ,152	,142	,133	,125	,118	,112	,106	,101	,96
        ,91	,87	,84	,80	,77	,74	,71	,68	,66};

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

