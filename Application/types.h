// *****************************************************************************
// Filename    : types.h
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : types definition header file
//
// License     : GNU General Public License v3.0
//
// *****************************************************************************

#ifndef TYPES_2017_12_27_H
#define TYPES_2017_12_27_H

typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned int uint32;
typedef signed int sint32;
typedef unsigned long long uint64;
typedef signed long long sint64;

typedef void (*pFunc)(void);
typedef void (*pTaskFunc)(void);

typedef enum
{
  FALSE,
  TRUE
}
boolean;

#endif // TYPES_2017_12_27_H
