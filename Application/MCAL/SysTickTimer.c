// *****************************************************************************
// Filename    : SysTickTimer.c
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
// Description : System tick timer Implementation
//
// License     : GNU General Public License v3.0
//
// *****************************************************************************

#include <SysTickTimer.h>

volatile uint64 SysTickTimer_MicrosecondTick;

stStkCtrl* StkCtrl = (stStkCtrl*)STK_CTRL_PTR;

void SysTickTimer_Init(void)
{
  STK_CTRL = 0;
  STK_LOAD = SYS_TICK_1MS;
  STK_VAL  = 0;
  StkCtrl->ClockSrc = SYS_TICK_CLKSRC_AHB;
  StkCtrl->TickInt = SYS_TICK_ENABLE_INT;
}

extern void mcal_gpt_init(void);

void SysTickTimer_Start(void)
{
  StkCtrl->Enable = SYS_TICK_ENABLE_TIMER;

  mcal_gpt_init();
}

uint64 SysTickTimer_ConsistentMicrosecondTick(void)
{
  // Return the system tick using a multiple read to ensure data consistency.

  // Do the first read of the sys-tick counter and the sys-tick
  // value. Handle reverse counting for sys-tick counter, which is
  // counting down.

  const uint32 sys_tick_counter_1 = (uint32) ((uint32) SYS_TICK_1MS - (uint32) STK_VAL);

  const uint64 sys_tick_value = SysTickTimer_MicrosecondTick;

  // Do the second read of the sys-tick counter and the sys-tick
  // value. Handle reverse counting for sys-tick counter, which is
  // counting down.

  const uint32 sys_tick_counter_2 = (uint32) ((uint32) SYS_TICK_1MS - (uint32) STK_VAL);

  // Perform the consistency check.

  const uint64 sys_tick_consistent_value =
    (uint64)
    (
      (sys_tick_counter_2 >= sys_tick_counter_1)
        ? (uint64) (sys_tick_value               + sys_tick_counter_1)
        : (uint64) (SysTickTimer_MicrosecondTick + sys_tick_counter_2)
    );

  // Perform scaling (without rounding correction).
  return (uint64) (sys_tick_consistent_value / 24U);
}
