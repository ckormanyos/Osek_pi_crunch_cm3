///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2012 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_spi.h>
#include <mcal_spi_types.h>

namespace local
{
  auto mcal_spi_spi_lcd_init() -> void;
  auto mcal_spi_spi_sram_init() -> void;

  auto mcal_spi_spi_lcd_init() -> void
  {
    mcal::spi::mcal_spi_lcd_type::init();
  }

  auto mcal_spi_spi_sram_init() -> void
  {
    mcal::spi::mcal_spi_sram_type::init();
  }
} // namespace local

auto mcal::spi::init(const config_type*) -> void
{
  local::mcal_spi_spi_sram_init();
  local::mcal_spi_spi_lcd_init();
}
