///////////////////////////////////////////////////////////////////////////////
//  Copyright Iliass Mahjoub 2023 - 2024
//  Copyright Christopher Kormanyos 2024
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_lcd.h>
#include <mcal_lcd/mcal_lcd_serlcd_sparkfun.h>
#include <mcal_spi_types.h>

namespace local_lcd
{
  auto lcd_sparkfun() -> mcal::lcd::lcd_base&;

  auto lcd_sparkfun() -> mcal::lcd::lcd_base&
  {
    static mcal::lcd::lcd_serlcd_sparkfun<mcal::spi::mcal_spi_lcd_type> my_lcd;

    return my_lcd;
  }
} // namespace local_lcd;

extern "C"
{
  void mcal_lcd_init(void)
  {
    static const bool is_init { local_lcd::lcd_sparkfun().init() };

    static_cast<void>(is_init);
  }

  void mcal_lcd_write_line(const char* StringToPrint, const size_t StringSize, const size_t LineIndex)
  {
    static_cast<void>
    (
      local_lcd::lcd_sparkfun().write(StringToPrint, StringSize, static_cast<std::uint_fast8_t>(LineIndex))
    );
  }
}
