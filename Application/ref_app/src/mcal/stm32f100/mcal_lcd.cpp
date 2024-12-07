///////////////////////////////////////////////////////////////////////////////
//  Copyright Iliass Mahjoub 2023 - 2024
//  Copyright Christopher Kormanyos 2024
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_lcd.h>
#include <mcal_lcd/mcal_lcd_serlcd_sparkfun.h>
#include <mcal_port.h>
#include <mcal_spi/mcal_spi_software_port_driver.h>

namespace local_lcd
{
  using port_pin_csn_type = mcal::port::port_pin<std::uint32_t,
                                                 std::uint32_t,
                                                 mcal::reg::gpiob_odr,
                                                 static_cast<std::uint32_t>(UINT32_C(2))>;

  using port_pin_sdo_type = mcal::port::port_pin<std::uint32_t,
                                                 std::uint32_t,
                                                 mcal::reg::gpiob_odr,
                                                 static_cast<std::uint32_t>(UINT32_C(7))>;

  using port_pin_sdi_type = mcal::port::port_pin<std::uint32_t,
                                                 std::uint32_t,
                                                 mcal::reg::gpiob_odr,
                                                 static_cast<std::uint32_t>(UINT32_C(1))>;

  using port_pin_sck_type = mcal::port::port_pin<std::uint32_t,
                                                 std::uint32_t,
                                                 mcal::reg::gpiob_odr,
                                                 static_cast<std::uint32_t>(UINT32_C(0))>;

  auto spi_lcd() -> util::communication_base&;

  auto spi_lcd() -> util::communication_base&
  {
    using spi_lcd_type = mcal::spi::spi_software_port_driver<local_lcd::port_pin_sck_type,
                                                             local_lcd::port_pin_sdi_type,
                                                             local_lcd::port_pin_csn_type,
                                                             local_lcd::port_pin_sdo_type,
                                                             static_cast<std::uint_fast16_t>(UINT8_C(2)),
                                                             true>;

    static spi_lcd_type my_spi { };

    return my_spi;
  }

  auto lcd_sparkfun() -> mcal::lcd::lcd_base&;

  auto lcd_sparkfun() -> mcal::lcd::lcd_base&
  {
    static mcal::lcd::lcd_serlcd_sparkfun my_lcd(spi_lcd());

    return my_lcd;
  }
} // namespace local_lcd;

extern "C"
{
  void mcal_spi_spi_lcd_init(void);

  void mcal_spi_spi_lcd_init(void)
  {
    local_lcd::spi_lcd().init();
  }

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
