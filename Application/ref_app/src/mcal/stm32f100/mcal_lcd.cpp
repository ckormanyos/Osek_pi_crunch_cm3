///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_lcd.h>
#include <mcal_lcd/mcal_lcd_serlcd_sparkfun.h>
#include <mcal_port.h>

///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2023 - 2024.                        //
//  Copyright Christopher Kormanyos 2024.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

#include <mcal_lcd/mcal_lcd_serlcd_sparkfun.h>
#include <mcal_spi/mcal_spi_software_port_driver.h>

namespace local_cdd_spi
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
} // namespace local_cdd_spi;

namespace
{
  auto cdd_spi_channel() -> util::communication_buffer_depth_one_byte&;

  auto cdd_spi_channel() -> util::communication_buffer_depth_one_byte&
  {
    using cdd_spi_channel_type = mcal::spi::spi_software_port_driver<local_cdd_spi::port_pin_sck_type,
                                                                     local_cdd_spi::port_pin_sdi_type,
                                                                     local_cdd_spi::port_pin_csn_type,
                                                                     local_cdd_spi::port_pin_sdo_type,
                                                                     static_cast<std::uint_fast16_t>(UINT8_C(2)),
                                                                     true>;

    static cdd_spi_channel_type com_cdd_spi { };

    return com_cdd_spi;
  }
}

auto mcal::lcd::lcd0() -> mcal::lcd::lcd_base&
{
  static mcal::lcd::lcd_serlcd_sparkfun my_lcd(cdd_spi_channel());

  return my_lcd;
}

extern "C"
{
  void mcal_lcd_init(void);
  void mcal_lcd_write_line(const char* StringToPrint, const size_t StringSize, const size_t LineIndex);

  void mcal_lcd_init(void)
  {
    static_cast<void>(mcal::lcd::lcd0().init());
  }

  void mcal_lcd_write_line(const char* StringToPrint, const size_t StringSize, const size_t LineIndex)
  {
    static_cast<void>
    (
      mcal::lcd::lcd0().write(StringToPrint, StringSize, static_cast<std::uint_fast8_t>(LineIndex))
    );
  }
}
