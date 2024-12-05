///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2023 - 2024.                        //
//  Copyright Christopher Kormanyos 2024.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

#ifndef CDD_SERLCD
  #define CDD_SERLCD

  #include <stddef.h>

  #if defined(__cplusplus)
  extern "C"
  {
  #endif

  void CddSerLCD_Init(void);
  void CddSerLCD_WriteLine(const char* StringToPrint, const size_t StringSize, const size_t LineIndex);

  #if defined(__cplusplus)
  }
  #endif

#endif /* CDD_SERLCD */
