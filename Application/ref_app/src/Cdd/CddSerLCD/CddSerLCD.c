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

static void CddSerLcd_msDelays(const unsigned ms_count)
{
  UtilTimer_BlockingDelay(ms_count);
}

static void CddSerLcd_ClearLCD(void)
{
  /* Wait 500ms */
  CddSerLcd_msDelays(500U);

  /* Send clear display command */
  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(CDD_SERLCD_SETTING_MODE);
  CddSpi_CsDisable();
  CddSerLcd_msDelays(1U);

  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(CDD_SERLCD_CLEAR_DISPLAY);
  CddSpi_CsDisable();
  CddSerLcd_msDelays(1U);
}

static void CddSerLcd_SetBacklight(void)
{
  /* Set Backlight */
  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(CDD_SERLCD_SETTING_MODE);
  CddSpi_CsDisable();
  CddSerLcd_msDelays(1U);

  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(CDD_SERLCD_GREEN_BACKLIGHT);
  CddSpi_CsDisable();
  CddSerLcd_msDelays(1U);
}

static void CddSerLcd_SetLCDSize(void)
{
  /* Change lines count to 4 */

  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(CDD_SERLCD_SETTING_MODE);
  CddSpi_CsDisable();
  CddSerLcd_msDelays(1U);

  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(CDD_SERLCD_SIZE_LINES_04);
  CddSpi_CsDisable();
  CddSerLcd_msDelays(1U);

  /* Change width count to 20 */

  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(CDD_SERLCD_SETTING_MODE);
  CddSpi_CsDisable();
  CddSerLcd_msDelays(1U);

  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(CDD_SERLCD_SIZE_WIDTH_20);
  CddSpi_CsDisable();
  CddSerLcd_msDelays(1U);
}

static void CddSerLcd_SelectLine(const size_t LineIndexToUse)
{
  /* To set the active cursor position, send the control character 254 followed by 128 + row + position */
  /* OpenLCD.write(254); */
  /* OpenLCD.write(128 + 64 + 9); //Change the position (128) of the cursor to 2nd row (64), position 9 (9) */

  static const uint8_t IndexToRowTable[4U] = { 0U, 64U, 20U, 84U };

  /* Chip select enable */
  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(0xFEU);
  CddSpi_CsDisable();
  CddSerLcd_msDelays(1U);

  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(0x80U + IndexToRowTable[LineIndexToUse] + 0x00);
  CddSpi_CsDisable();
  CddSerLcd_msDelays(1U);
}

void CddSerLCD_Init(void)
{
  CddSerLcd_ClearLCD();

  CddSerLcd_SetBacklight();

  CddSerLcd_SetLCDSize();
}

void CddSerLCD_WriteLine(const char* StringToPrint, const size_t StringSize, const size_t LineIndex)
{
  // Limit to the maximum width of the display width.
  const size_t LineIndexToUse = ((LineIndex > (size_t) 3U) ? (size_t) 3U : LineIndex);

  // Limit to the maximum width of the display width.
  const size_t SizeToWrite = ((StringSize > (size_t) 20U) ? (size_t) 20U : StringSize);

  CddSerLcd_SelectLine(LineIndexToUse);

  for(size_t idx = (size_t) 0U; idx < (size_t) 20U; ++idx)
  {
    const char CharToWrite = ((idx < SizeToWrite) ? StringToPrint[idx] : ' ');

    /* Chip select enable */
    CddSpi_CsEnable();

    /* Send next character. */
    CddSpi_TransferSingleByte(CharToWrite);

    /* Chip select disable */
    CddSpi_CsDisable();

    CddSerLcd_msDelays(1U);
  }
}
