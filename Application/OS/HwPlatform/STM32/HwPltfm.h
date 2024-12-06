// *****************************************************************************
// Filename    : HwPltfm.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : Hardware Platform definition for ARM Cortex-M3
//
// License     : GNU General Public License v3.0
//  
// *****************************************************************************

#ifndef HARDWARE_PLATFORM_2017_12_27_H
#define HARDWARE_PLATFORM_2017_12_27_H

#if 0
#define ENABLE_IRQ(x)(*(volatile unsigned int*)(0xE000E100)) |= (unsigned int) (1ULL << (x))
#define TRAP(x)      (*(volatile unsigned int*)(0xE000EF00)) = (unsigned int) (x)
#define OS_DISPATCH() ENABLE_IRQ(6);TRAP(6)
#endif

#define SCB_ICSR 0xE000ED04UL
#define SET_PENDSV()   (*(volatile unsigned int*)(SCB_ICSR)) |= (unsigned int) (1ULL << 28U)
#define CLEAR_PENDSV() (*(volatile unsigned int*)(SCB_ICSR))
#define OS_DISPATCH() SET_PENDSV()

#if defined(__GNUC__)
#define ENABLE_INTERRUPTS()  __asm("CPSIE I")
#define DISABLE_INTERRUPTS() __asm("CPSID I")
#else
#define ENABLE_INTERRUPTS()    __asm{CPSIE I}
#define DISABLE_INTERRUPTS()  __asm{CPSID I}
#endif

#define EXCEPTION_CODE_MASK   (uint32) 0x1FU

#endif // HARDWARE_PLATFORM_2017_12_27_H
