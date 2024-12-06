///////////////////////////////////////////////////////////////////////////////
//  Copyright Iliass Mahjoub 2023 - 2024
//  Copyright Christopher Kormanyos 2024
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_LCD_SERLCD_SPARKFUN_2024_12_05_H // NOLINT(llvm-header-guard)
  #define MCAL_LCD_SERLCD_SPARKFUN_2024_12_05_H

  #include <mcal_lcd/mcal_lcd_base.h>

  #include <util/utility/util_communication.h>
  #include <util/utility/util_time.h>

  #if(__cplusplus >= 201703L)
  namespace mcal::lcd {
  #else
  namespace mcal { namespace lcd { // NOLINT(modernize-concat-nested-namespaces)
  #endif

  class lcd_serlcd_sparkfun final : public mcal::lcd::lcd_base
  {
  private:
    using timer_type = util::timer<std::uint32_t>;
    using tick_type = typename timer_type::tick_type;

    static constexpr std::uint8_t CDD_SERLCD_BLUE_BACKLIGHT      { (uint8_t) UINT8_C(0xD9) };
    static constexpr std::uint8_t CDD_SERLCD_GREEN_BACKLIGHT     { (uint8_t) UINT8_C(0xBB) };
    static constexpr std::uint8_t CDD_SERLCD_PRIMARY_BACKLIGHT   { (uint8_t) UINT8_C(0x9D) };

    static constexpr std::uint8_t CDD_SERLCD_SETTING_MODE        { (uint8_t) UINT8_C(0x7C) };
    static constexpr std::uint8_t CDD_SERLCD_CLEAR_DISPLAY       { (uint8_t) UINT8_C(0x2D) };

    static constexpr std::uint8_t CDD_SERLCD_SIZE_WIDTH_20       { (uint8_t) UINT8_C(0x03) };
    static constexpr std::uint8_t CDD_SERLCD_SIZE_LINES_04       { (uint8_t) UINT8_C(0x05) };
    static constexpr std::uint8_t CDD_SERLCD_SIZE_LINES_02       { (uint8_t) UINT8_C(0x06) };
    static constexpr std::uint8_t CDD_SERLCD_SIZE_LINES_01       { (uint8_t) UINT8_C(0x07) };

  public:
    explicit lcd_serlcd_sparkfun(util::communication_buffer_depth_one_byte& com)
      : my_com(com) { }

    lcd_serlcd_sparkfun() = delete;

    ~lcd_serlcd_sparkfun() override = default;

    auto init(void) -> bool override
    {
      blocking_delay(timer_type::milliseconds(tick_type { UINT16_C(1250) }));

      CddSerLcd_Setting(CDD_SERLCD_CLEAR_DISPLAY);
      CddSerLcd_Setting(CDD_SERLCD_GREEN_BACKLIGHT);
      CddSerLcd_Setting(CDD_SERLCD_SIZE_LINES_04);
      CddSerLcd_Setting(CDD_SERLCD_SIZE_WIDTH_20);

      return true;
    }

    auto write(const char* StringToPrint, const std::size_t StringSize, const std::uint_fast8_t LineIndex) -> bool override
    {
      // Limit to the maximum width of the display width.
      const size_t LineIndexToUse = ((LineIndex > std::size_t { UINT8_C(3) }) ? std::size_t { UINT8_C(3) } : LineIndex);

      // Limit to the maximum width of the display width.
      const size_t SizeToWrite = ((StringSize > std::size_t { UINT8_C(20)}) ? std::size_t { UINT8_C(20) } : StringSize);

      CddSerLcd_SelectLine(LineIndexToUse);

      for(std::size_t idx { UINT8_C(0) }; idx < std::size_t { UINT8_C(20) }; ++idx)
      {
        const char CharToWrite = ((idx < SizeToWrite) ? StringToPrint[idx] : ' ');

        CddSerLcd_Transfer(CharToWrite);
      }

      return true;
    }

  private:
    util::communication_buffer_depth_one_byte& my_com;

    static void blocking_delay(const tick_type blocking_delay_value)
    {
      timer_type::blocking_delay(blocking_delay_value);
    }

    void CddSerLcd_Transfer(const uint8_t by)
    {
      my_com.select();
      static_cast<void>(my_com.send(by));
      my_com.deselect();
      blocking_delay(timer_type::microseconds(tick_type { UINT8_C(150) }));
    }

    void CddSerLcd_Setting(const uint8_t setting)
    {
      CddSerLcd_Transfer(CDD_SERLCD_SETTING_MODE);
      CddSerLcd_Transfer(setting);
    }

    void CddSerLcd_SelectLine(const size_t LineIndexToUse)
    {
      // To set the active cursor position, send the control character 254 followed by 128 + row + position.
      // OpenLCD.write(254);

      // Change the position (128) of the cursor to 2nd row (64), position 9 (9) */
      // OpenLCD.write(128 + 64 + 9);

      constexpr uint8_t IndexToRowTable[std::size_t { UINT8_C(4) }] =
      {
        std::uint8_t { UINT8_C( 0) },
        std::uint8_t { UINT8_C(64) },
        std::uint8_t { UINT8_C(20) },
        std::uint8_t { UINT8_C(84) }
      };

      CddSerLcd_Transfer(std::uint8_t { UINT8_C(0xFE) });

      const std::uint8_t
        by_row
        {
          static_cast<std::uint8_t>
          (
            UINT8_C(0x80) + IndexToRowTable[LineIndexToUse] + UINT8_C(0x00)
          )
        };

      CddSerLcd_Transfer(by_row);
    }
  };

  #if(__cplusplus >= 201703L)
  } // namespace mcal::lcd
  #else
  } // namespace lcd
  } // namespace mcal
  #endif

#endif // MCAL_LCD_SERLCD_SPARKFUN_2024_12_05_H
