///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_memory_sram.h>
#include <mcal_memory/mcal_memory_sram_generic_spi.h>
#include <mcal_spi.h>

auto mcal::memory::sram::mcal_memory_sram_device() -> mcal::memory::sram::mcal_memory_sram_device_type&
{
  static mcal_memory_sram_device_type my_device(mcal::spi::spi_channels());

  return my_device;
}
