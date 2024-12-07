///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2024
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_SPI_TYPES_2024_12_07_H
  #define MCAL_SPI_TYPES_2024_12_07_H

  #include <mcal_port.h>
  #include <mcal_reg.h>
  #include <mcal_spi/mcal_spi_software_port_driver.h>

  namespace mcal { namespace spi {

  namespace sram {

  // Pin 1 on SRAM chip.
  using port_pin_sram_csn0_type = mcal::port::port_pin<std::uint32_t,
                                                       std::uint32_t,
                                                       mcal::reg::gpioa_odr,
                                                       static_cast<std::uint32_t>(UINT32_C(11))>;

  // Pin 2 on SRAM chip.
  using port_pin_sram_miso_type = mcal::port::port_pin<std::uint32_t,
                                                       std::uint32_t,
                                                       mcal::reg::gpioa_odr,
                                                       static_cast<std::uint32_t>(UINT32_C(10))>;

  // Pin 6 on SRAM chip.
  using port_pin_sram_sck__type = mcal::port::port_pin<std::uint32_t,
                                                       std::uint32_t,
                                                       mcal::reg::gpioa_odr,
                                                       static_cast<std::uint32_t>(UINT32_C(9))>;

  // Pin 5 on SRAM chip.
  using port_pin_sram_mosi_type = mcal::port::port_pin<std::uint32_t,
                                                       std::uint32_t,
                                                       mcal::reg::gpioa_odr,
                                                       static_cast<std::uint32_t>(UINT32_C(8))>;

  using mcal_spi_sram_type =
    mcal::spi::spi_software_port_driver<port_pin_sram_sck__type,
                                        port_pin_sram_mosi_type,
                                        port_pin_sram_csn0_type,
                                        port_pin_sram_miso_type,
                                        static_cast<std::uint_fast16_t>(UINT8_C(0)),
                                        true>;

  } // namespace sram

  namespace lcd {

  using port_pin_lcd_csn_type = mcal::port::port_pin<std::uint32_t,
                                                     std::uint32_t,
                                                     mcal::reg::gpiob_odr,
                                                     static_cast<std::uint32_t>(UINT32_C(2))>;

  using port_pin_lcd_sdo_type = mcal::port::port_pin<std::uint32_t,
                                                     std::uint32_t,
                                                     mcal::reg::gpiob_odr,
                                                     static_cast<std::uint32_t>(UINT32_C(7))>;

  using port_pin_lcd_sdi_type = mcal::port::port_pin<std::uint32_t,
                                                     std::uint32_t,
                                                     mcal::reg::gpiob_odr,
                                                     static_cast<std::uint32_t>(UINT32_C(1))>;

  using port_pin_lcd_sck_type = mcal::port::port_pin<std::uint32_t,
                                                     std::uint32_t,
                                                     mcal::reg::gpiob_odr,
                                                     static_cast<std::uint32_t>(UINT32_C(0))>;

  using mcal_spi_lcd_type = mcal::spi::spi_software_port_driver<port_pin_lcd_sck_type,
                                                                port_pin_lcd_sdi_type,
                                                                port_pin_lcd_csn_type,
                                                                port_pin_lcd_sdo_type,
                                                                static_cast<std::uint_fast16_t>(UINT8_C(2)),
                                                                true>;
  } // namespace lcd

  } // namespace spi
  } // namespace mcal

#endif // MCAL_SPI_TYPES_2024_12_07_H
