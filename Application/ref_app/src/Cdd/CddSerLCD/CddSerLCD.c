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
  TimerBlockingDelay(ms_count);
}

static void CddSerLcd_ClearLCD(void)
{
  /* Wait 500ms */
  CddSerLcd_msDelays(500U);
  /* Chip select enable */
  CddSpi_CsEnable();

  /* Send clear display command */
  CddSpi_TransferSingleByte(CDD_SERLCD_SETTING_MODE);
  CddSerLcd_msDelays(4U);
  CddSpi_TransferSingleByte(CDD_SERLCD_CLEAR_DISPLAY);
  CddSerLcd_msDelays(4U);

 /* Chip select disable */
  CddSpi_CsDisable();
}

static void CddSerLcd_SetBlueBacklight(void)
{
  /* Chip select enable */
  CddSpi_CsEnable();

  /* Set Backlight into Blue */
  CddSpi_TransferSingleByte(CDD_SERLCD_SETTING_MODE);
  CddSerLcd_msDelays(4U);
  CddSpi_TransferSingleByte(CDD_SERLCD_GREEN_BACKLIGHT);
  CddSerLcd_msDelays(4U);

 /* Chip select disable */
  CddSpi_CsDisable();
}

static void CddSerLcd_SetLCDSize(void)
{
  /* Change lines count to 4 */
  /* Chip select enable */
  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(CDD_SERLCD_SETTING_MODE);
  CddSerLcd_msDelays(4U);
  CddSpi_TransferSingleByte(CDD_SERLCD_SIZE_LINES_04);
  CddSerLcd_msDelays(4U);
 /* Chip select disable */
  CddSpi_CsDisable();

  /* Change width count to 20 */
  /* Chip select enable */
  CddSpi_CsEnable();
  CddSpi_TransferSingleByte(CDD_SERLCD_SETTING_MODE);
  CddSerLcd_msDelays(4U);
  CddSpi_TransferSingleByte(CDD_SERLCD_SIZE_WIDTH_20);
  CddSerLcd_msDelays(4U);
 /* Chip select disable */
  CddSpi_CsDisable();
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
  CddSerLcd_msDelays(4U);

  CddSpi_TransferSingleByte(0x80U + IndexToRowTable[LineIndexToUse] + 0x00);
  CddSerLcd_msDelays(4U);

 /* Chip select disable */
  CddSpi_CsDisable();
}

void CddSerLCD_Init(void)
{
  /* Clear display */
  CddSerLcd_ClearLCD();

  /* Set blue backlight */
  CddSerLcd_SetBlueBacklight();

  /* Set blue backlight */
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
