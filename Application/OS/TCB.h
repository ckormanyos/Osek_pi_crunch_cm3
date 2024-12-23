// *****************************************************************************
// Filename    : TCB.h
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
// Description : Task Control Block definition header file
//
// License     : GNU General Public License v3.0
//  
// *****************************************************************************

#ifndef TCB_2017_12_27_H
#define TCB_2017_12_27_H

#include <OsTypes.h>

/***************************************************/
/*            X Macro System Generation            */
/***************************************************/
#define OS_GEN_ENUM
#include <OsCfg.h>
#undef OS_GEN_ENUM

#define OS_GEN_FUNC_PROTO
#include <OsCfg.h>
#undef OS_GEN_FUNC_PROTO

/* OS Config */
extern volatile Ocb_t OCB_Cfg;

#define STARTUPHOOK   0
#define ERRORHOOK     0
#define SHUTDOWNHOOK  0
#define PRETASKHOOK   0
#define POSTTASKHOOK  0

#define RES_SCHEDULER_PRIO (uint32)0x7FFFFFFFu
#define RES_TRACE_PRIO     (uint32)1u

#endif // TCB_2017_12_27_H
