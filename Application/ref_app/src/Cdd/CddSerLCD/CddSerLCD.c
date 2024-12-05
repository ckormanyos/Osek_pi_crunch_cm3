///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2023 - 2024.                        //
//  Copyright Christopher Kormanyos 2024.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

#include <Cdd/CddSerLCD/CddSerLCD.h>
#include <Cdd/CddSpi/CddSpi.h>

#include <Util/UtilTimer.h>

#include <stdint.h>

#define CDD_SERLCD_BLUE_BACKLIGHT       (uint8_t) UINT8_C(0xD9)
#define CDD_SERLCD_GREEN_BACKLIGHT      (uint8_t) UINT8_C(0xBB)
#define CDD_SERLCD_PRIMARY_BACKLIGHT    (uint8_t) UINT8_C(0x9D)

#define CDD_SERLCD_SETTING_MODE         (uint8_t) UINT8_C(0x7C)
#define CDD_SERLCD_CLEAR_DISPLAY        (uint8_t) UINT8_C(0x2D)

#define CDD_SERLCD_SIZE_WIDTH_20        (uint8_t) UINT8_C(0x03)
#define CDD_SERLCD_SIZE_LINES_04        (uint8_t) UINT8_C(0x05)
#define CDD_SERLCD_SIZE_LINES_02        (uint8_t) UINT8_C(0x06)
#define CDD_SERLCD_SIZE_LINES_01        (uint8_t) UINT8_C(0x07)

static void CddSerLcd_micro_sec_Delay(const unsigned us_count);
static void CddSerLcd_milli_sec_Delay(const unsigned ms_count);
static void CddSerLcd_Transfer(const uint8_t by);
static void CddSerLcd_SetSetting(const uint8_t setting);
static void CddSerLcd_ClearLCD(void);
static void CddSerLcd_SetBacklight(void);
static void CddSerLcd_SetLCDSize(void);
static void CddSerLcd_SelectLine(const size_t LineIndexToUse);

void CddSerLCD_Init(void)
{
  CddSerLcd_ClearLCD();
  CddSerLcd_SetBacklight();
  CddSerLcd_SetLCDSize();
}

void CddSerLCD_WriteLine(const char* StringToPrint, const size_t StringSize, const size_t LineIndex)
{
  // Limit to the maximum width of the display width.
  const size_t LineIndexToUse = ((LineIndex > (size_t) UINT8_C(3)) ? (size_t) UINT8_C(3) : LineIndex);

  // Limit to the maximum width of the display width.
  const size_t SizeToWrite = ((StringSize > (size_t) UINT8_C(20)) ? (size_t) UINT8_C(20) : StringSize);

  CddSerLcd_SelectLine(LineIndexToUse);

  for(size_t idx = (size_t) UINT8_C(0); idx < (size_t) UINT8_C(20); ++idx)
  {
    const char CharToWrite = ((idx < SizeToWrite) ? StringToPrint[idx] : ' ');

    CddSpi_CsEnable();
    CddSpi_TransferSingleByte(CharToWrite);
    CddSpi_CsDisable();
    CddSerLcd_micro_sec_Delay((unsigned) UINT8_C(150));
  }
}

static void CddSerLcd_micro_sec_Delay(const unsigned us_count) { UtilTimer_BlockingDelayMicro((uint64_t) us_count); }
static void CddSerLcd_milli_sec_Delay(const unsigned ms_count) { UtilTimer_BlockingDelayMicro((uint64_t) ((uint64_t) ms_count * 1000U)); }

static void CddSerLcd_Transfer(const uint8_t by)
{
  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(by);
  CddSpi_CsDisable();
  CddSerLcd_micro_sec_Delay((unsigned) UINT8_C(150));
}

static void CddSerLcd_SetSetting(const uint8_t setting)
{
  CddSerLcd_Transfer(CDD_SERLCD_SETTING_MODE);
  CddSerLcd_Transfer(setting);
}

static void CddSerLcd_ClearLCD(void)
{
  CddSerLcd_milli_sec_Delay((unsigned) UINT16_C(500));

  CddSerLcd_SetSetting(CDD_SERLCD_CLEAR_DISPLAY);
}

static void CddSerLcd_SetBacklight(void)
{
  CddSerLcd_SetSetting(CDD_SERLCD_GREEN_BACKLIGHT);
}

static void CddSerLcd_SetLCDSize(void)
{
  CddSerLcd_SetSetting(CDD_SERLCD_SIZE_LINES_04);
  CddSerLcd_SetSetting(CDD_SERLCD_SIZE_WIDTH_20);
}

static void CddSerLcd_SelectLine(const size_t LineIndexToUse)
{
  // To set the active cursor position, send the control character 254 followed by 128 + row + position.
  // OpenLCD.write(254);

  // Change the position (128) of the cursor to 2nd row (64), position 9 (9) */
  // OpenLCD.write(128 + 64 + 9);

  static const uint8_t IndexToRowTable[(size_t) UINT8_C(4)] =
  {
    (uint8_t) UINT8_C( 0),
    (uint8_t) UINT8_C(64),
    (uint8_t) UINT8_C(20),
    (uint8_t) UINT8_C(84)
  };

  CddSerLcd_Transfer((uint8_t) UINT8_C(0xFE));

  CddSerLcd_Transfer((uint8_t) (UINT8_C(0x80) + IndexToRowTable[LineIndexToUse] + UINT8_C(0x00)));
}
