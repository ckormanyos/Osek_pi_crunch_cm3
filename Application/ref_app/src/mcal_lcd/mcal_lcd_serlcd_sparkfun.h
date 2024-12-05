///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2024.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_LCD_SERLCD_SPARKFUN_2024_12_05_H // NOLINT(llvm-header-guard)
  #define MCAL_LCD_SERLCD_SPARKFUN_2024_12_05_H

  #include <mcal_lcd/mcal_lcd_base.h>

  #include <Util/utility/util_communication.h>
  #include <util/utility/util_time.h>

  #include <algorithm>
  #include <cstring>

  #if(__cplusplus >= 201703L)
  namespace mcal::lcd {
  #else
  namespace mcal { namespace lcd { // NOLINT(modernize-concat-nested-namespaces)
  #endif

  class lcd_serlcd_sparkfun final : public mcal::lcd::lcd_base
  {
  private:
    using timer_type = util::timer<std::uint32_t>;

  public:
    explicit lcd_serlcd_sparkfun(util::communication_buffer_depth_one_byte& com)
      : my_com(com) { }

    lcd_serlcd_sparkfun() = delete;

    ~lcd_serlcd_sparkfun() override = default;

    auto init(void) -> bool override
    {
      return true;
    }

    auto write(const char* pstr,
               const std::size_t length,
               const std::uint_fast8_t line_index) -> bool override
    {
      static_cast<void>(pstr);
      static_cast<void>(length);
      static_cast<void>(line_index);

      return true;
    }

  private:
    util::communication_buffer_depth_one_byte& my_com;

    static void blocking_delay(const typename timer_type::tick_type blocking_delay_value)
    {
      timer_type::blocking_delay(blocking_delay_value);
    }

    auto set_line_index(const std::uint8_t index) -> bool override
    {
      static_cast<void>(index);

      return true;
    }
  };

  #if(__cplusplus >= 201703L)
  } // namespace mcal::lcd
  #else
  } // namespace lcd
  } // namespace mcal
  #endif

#endif // MCAL_LCD_SERLCD_SPARKFUN_2024_12_05_H
