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

  #include <stdint.h>
  #include <stddef.h>

  #if defined(__cplusplus)
  extern "C"
  {
  #endif

  #define CDD_SERLCD_BLUE_BACKLIGHT       (uint8_t)0xD9U
  #define CDD_SERLCD_GREEN_BACKLIGHT      (uint8_t)0xBBU
  #define CDD_SERLCD_PRIMARY_BACKLIGHT    (uint8_t)0x9DU

  #define CDD_SERLCD_SETTING_MODE         (uint8_t)0x7CU
  #define CDD_SERLCD_CLEAR_DISPLAY        (uint8_t)0x2DU

  #define CDD_SERLCD_SIZE_WIDTH_20        (uint8_t)0x03U
  #define CDD_SERLCD_SIZE_LINES_04        (uint8_t)0x05U
  #define CDD_SERLCD_SIZE_LINES_02        (uint8_t)0x06U
  #define CDD_SERLCD_SIZE_LINES_01        (uint8_t)0x07U

  void CddSerLCD_Init(void);
  void CddSerLCD_WriteLine(const char* StringToPrint, const size_t StringSize, const size_t LineIndex);

  #if defined(__cplusplus)
  }
  #endif

#endif /* CDD_SERLCD */
