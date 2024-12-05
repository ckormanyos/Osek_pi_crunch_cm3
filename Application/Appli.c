// *****************************************************************************
// Filename    : Appli.c
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
// Description : Main function
//
// License     : GNU General Public License v3.0
//
// *****************************************************************************

#include <Cdd/CddSerLCD/CddSerLCD.h>
#include <OsAPIs.h>
#include <TCB.h>

#include <stdbool.h>
#include <stdint.h>

static bool pi_result_is_ok = true;

int main(void)
{
  extern void mcal_init(void);

  mcal_init();

  OS_StartOS(APP_MODE_DEFAULT);

  return(0);
}

TASK(T1)
{
  extern void pi_led_toggle(void);

  pi_led_toggle();

  const OsEventMaskType OsWaitEventMask = (OsEventMaskType) EVT_BLINK_LED;

  (void) OS_SetRelAlarm(ALARM_BLINK_LED, (OsTickType) UINT8_C(0), (OsTickType) UINT16_C(997));

  for(;;)
  {
    if(E_OK == OS_WaitEvent(OsWaitEventMask))
    {
      OsEventMaskType Events = (OsEventMaskType) UINT8_C(0);

      (void) OS_GetEvent((OsTaskType) T1, &Events);

      if((Events & EVT_BLINK_LED) == EVT_BLINK_LED)
      {
        OS_ClearEvent(EVT_BLINK_LED);

        pi_led_toggle();
      }
    }
    else
    {
      OS_TerminateTask();
    }

    if(!pi_result_is_ok)
    {
      OS_TerminateTask();
    }
  }
}

TASK(Idle)
{
  CddSerLCD_Init();

  for(;;)
  {
    extern int pi_main(void);

    const int pi_result = pi_main();

    pi_result_is_ok = ((pi_result == (int) INT8_C(0)) && pi_result_is_ok);

    if(!pi_result_is_ok)
    {
      OS_TerminateTask();
    }
  }
}
