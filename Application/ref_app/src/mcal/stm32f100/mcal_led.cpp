///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_irq.h>
#include <mcal_led.h>
#include <mcal_led/mcal_led_port.h>

extern "C"
auto mcal_led_toggle(void) -> void;

extern "C"
auto mcal_led_toggle(void) -> void
{
  mcal::irq::disable_all();

  mcal::led::led0().toggle();
  mcal::led::led1().toggle();

  mcal::irq::enable_all();
}

mcal::led::led_base& mcal::led::led0()
{
  using led0_port_type = mcal::port::port_pin<std::uint32_t,
                                              std::uint32_t,
                                              mcal::reg::gpioc_odr,
                                              UINT32_C(8)>;

  using led0_led_type = mcal::led::led_port<led0_port_type>;

  static led0_led_type my_led_00;

  return my_led_00;
}

mcal::led::led_base& mcal::led::led1()
{
  using led1_port_type = mcal::port::port_pin<std::uint32_t,
                                              std::uint32_t,
                                              mcal::reg::gpioc_odr,
                                              UINT32_C(9)>;

  using led1_led_type = mcal::led::led_port<led1_port_type>;

  static led1_led_type my_led_01;

  return my_led_01;
}
