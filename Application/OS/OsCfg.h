// *****************************************************************************************************************
// Filename    : OsCfg.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : OSEK based application config
//
// License     : GNU General Public License v3.0
//  
// *****************************************************************************************************************

#include"OsGenMac.h"

OS_CONFIG_BEGIN

  OS_TASK_BEGIN
    OS_TASK_DEF(Idle ,1 ,1024 ,1 ,OS_AUTOSTART    ,EXTENDED ,FULL_PREEMPT)
    OS_TASK_DEF(T1   ,4 ,1024 ,1 ,OS_AUTOSTART    ,EXTENDED ,FULL_PREEMPT)
  OS_TASK_END

  OS_EVENT_BEGIN
   OS_EVENT_DEF(EVT_BLINK_LED        , 0x01)
  OS_EVENT_END

  OS_ALARM_BEGIN
    OS_ALARM_DEF(ALARM_BLINK_LED        ,ALARM_SET_EVENT ,EVT_BLINK_LED        ,ALARM_NO_AUTOSTART ,T1   ,0)
  OS_ALARM_END

  OS_RESOURCE_BEGIN
    OS_RESOURCE_DEF(RES_SCHEDULER ,RES_SCHEDULER_PRIO , {1,1})
  OS_RESOURCE_END

  OS_INTERRUPT_BEGIN
    OS_INTERRUPT_CAT2_DEF(15,SysTickTimer)
  OS_INTERRUPT_END

OS_CONFIG_END
