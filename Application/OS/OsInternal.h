// *****************************************************************************
// Filename    : OsInternal.h
// 
// OS          : OSEK 2.2.3  
// 
// CC          : ECC1/BCC1
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : OS internal services
//
// License     : GNU General Public License v3.0
//  
// *****************************************************************************

#ifndef OS_INTERNAL_2017_12_27_H
#define OS_INTERNAL_2017_12_27_H

#include <types.h>

extern void OsGetCurrentSP(volatile unsigned int* CurrentSpPtr);
extern void OsGetPSR(volatile unsigned int* CurrentPsr);

boolean OsIsInterruptContext(void);
boolean OsIsCat2IntContext(void);

#endif // OS_INTERNAL_2017_12_27_H
