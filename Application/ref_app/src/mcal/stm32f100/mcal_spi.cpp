///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2012 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstddef>
#include <cstdint>

#include <mcal_port.h>
#include <mcal_reg.h>
#include <mcal_spi.h>
#include <mcal_spi/mcal_spi_software_port_driver.h>

namespace local
{
  constexpr auto mcal_spi_channel_count = static_cast<std::size_t>(UINT8_C(1));

  // Pin 1 on SRAM chip.
  using port_pin_csn0_type = mcal::port::port_pin<std::uint32_t,
                                                  std::uint32_t,
                                                  mcal::reg::gpioa_odr,
                                                  static_cast<std::uint32_t>(UINT32_C(11))>;

  // Pin 2 on SRAM chip.
  using port_pin_miso_type = mcal::port::port_pin<std::uint32_t,
                                                  std::uint32_t,
                                                  mcal::reg::gpioa_odr,
                                                  static_cast<std::uint32_t>(UINT32_C(10))>;

  // Pin 6 on SRAM chip.
  using port_pin_sck__type = mcal::port::port_pin<std::uint32_t,
                                                  std::uint32_t,
                                                  mcal::reg::gpioa_odr,
                                                  static_cast<std::uint32_t>(UINT32_C(9))>;

  // Pin 5 on SRAM chip.
  using port_pin_mosi_type = mcal::port::port_pin<std::uint32_t,
                                                  std::uint32_t,
                                                  mcal::reg::gpioa_odr,
                                                  static_cast<std::uint32_t>(UINT32_C(8))>;
} // namespace local;

auto mcal::spi::spi_sram() -> util::communication_base&
{
  using mcal_spi_sram_type =
    mcal::spi::spi_software_port_driver<local::port_pin_sck__type,
                                        local::port_pin_mosi_type,
                                        local::port_pin_csn0_type,
                                        local::port_pin_miso_type,
                                        static_cast<std::uint_fast16_t>(UINT8_C(0)),
                                        true>;

  static mcal_spi_sram_type spi0 { };

  return spi0;
}

extern "C"
{
  void mcal_spi_spi_sram_init(void);

  void mcal_spi_spi_sram_init(void)
  {
    mcal::spi::spi_sram().init();
  }
}
