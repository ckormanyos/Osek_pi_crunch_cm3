///////////////////////////////////////////////////////////////////////////////
//  Copyright Amine Chalandi 2017
//  Copyright Christopher Kormanyos 2023 - 2024
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : SysTickTimer.h
// 
// Core        : STM32F100RB Cortex(R)-M3
// 
// Board       : STM32VLDISCOVERY
//
// Compiler    : gcc-arm-none-eabi (GCC) or ARM(R) Compiler v5.06 for uVision (Keil)
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

#if defined(__cplusplus)
extern "C"
{
#endif

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
#define SYS_TICK_1MS          (unsigned int)0x5DC0U - 1U

void SysTickTimer_Init(void);
void SysTickTimer_Start(void);
uint64 SysTickTimer_ConsistentMicrosecondTick(void);

extern volatile uint64 SysTickTimer_MicrosecondTick;

#if defined(__cplusplus)
}
#endif

#endif // SYSTICKTIMER_2017_12_27_H
