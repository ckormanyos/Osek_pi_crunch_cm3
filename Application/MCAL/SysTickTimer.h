// *****************************************************************************
// Filename    : SysTickTimer.h
// 
// Core        : STM32F100RB Cortex(R)-M3
// 
// Board       : STM32VLDISCOVERY
//
// Compiler    : ARM(R) Compiler v5.06 for uVision (Keil)
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 04.09.2017
// 
// Description : System tick timer header file
//
// License     : GNU General Public License v3.0
//
// *****************************************************************************
#ifndef SYSTICKTIMER_2017_12_27_H
#define SYSTICKTIMER_2017_12_27_H

#include <types.h>

#define SYS_TICK_BASE_REG (unsigned int)(0xE000E010)

#define STK_CTRL   *((volatile unsigned int*)(SYS_TICK_BASE_REG + 0x00))
#define STK_LOAD   *((volatile unsigned int*)(SYS_TICK_BASE_REG + 0x04))
#define STK_VAL    *((volatile unsigned int*)(SYS_TICK_BASE_REG + 0x08))
#define STK_CALIB  *((volatile unsigned int*)(SYS_TICK_BASE_REG + 0x0C))

#define STK_CTRL_PTR  ((volatile unsigned int*)(SYS_TICK_BASE_REG + 0x00))

typedef struct
{
  unsigned int Enable:1;
  unsigned int TickInt:1;
  unsigned int ClockSrc:1;
  unsigned int :13;
  unsigned int CountFlag:1;
  unsigned int :15;
}
stStkCtrl;

#define SYS_TICK_CLKSRC_AHB   1
#define SYS_TICK_ENABLE_INT   1
#define SYS_TICK_ENABLE_TIMER 1
#define SYS_TICK_1MS          (unsigned int)0x5DC0 - 1

void SysTickTimer_Init(void);
void SysTickTimer_Start(void);

#endif // SYSTICKTIMER_2017_12_27_H
