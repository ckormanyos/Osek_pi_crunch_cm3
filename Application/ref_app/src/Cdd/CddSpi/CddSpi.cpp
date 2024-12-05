///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2023 - 2024.                        //
//  Copyright Christopher Kormanyos 2024.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

#include <Cdd/CddSpi/CddSpi.h>
#include <mcal_spi/mcal_spi_software_port_driver.h>

#include <cstdint>

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

extern "C"
uint8_t CddSpi_TransferSingleByte(const uint8_t byte_to_transfer)
{
  static_cast<void>(cdd_spi_channel().send(byte_to_transfer));

  std::uint8_t byte_to_recv { };

  static_cast<void>(cdd_spi_channel().recv(byte_to_recv));

  return byte_to_recv;
}

extern "C"
void CddSpi_CsEnable()
{
  cdd_spi_channel().select();
}

extern "C"
void CddSpi_CsDisable()
{
  cdd_spi_channel().deselect();
}
