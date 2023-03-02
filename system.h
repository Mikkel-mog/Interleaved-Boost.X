/* 
 * File:   
 * Author: Mikkel B. Mogensen
 * Comments: 
 * Revision history: 1
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SYSTEM_H
#define	SYSTEM_H

#include <xc.h> // include processor files - each processor file is guarded.  


void ConfigureOscillator(void); /*Configuration of oscillator*/
void ConfigureWDT(void);
void ConfigureComparator(void);
void ConfigureADC(void);
void ConfigureTIMER(void);
void ConfigurePWM(void);
void ConfigureINTERRUPT(void);
void ConfigureFVR(void);
#endif	/* XC_HEADER_TEMPLATE_H */

