///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_benchmark.h>
#include <mcal_port.h>
#include <mcal_reg.h>

void mcal::port::init(const config_type*)
{
  // Power management: Enable power for I/O ports : porta, portb, portc.
  constexpr auto port_enr_val = 
    static_cast<std::uint32_t>
    (
      static_cast<std::uint32_t>
      (
          static_cast<std::uint32_t>(UINT8_C(1))
        | static_cast<std::uint32_t>(UINT8_C(2))
        | static_cast<std::uint32_t>(UINT8_C(4))
      ) << static_cast<unsigned>(UINT8_C(2))
    );

  mcal::reg::reg_access_static<std::uint32_t, std::uint32_t, mcal::reg::rcc_apb2enr, port_enr_val>::reg_or();

  benchmark::benchmark_port_type::set_direction_output();
}
