///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2024.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_gpt.h>

#include <SysTickTimer.h>

namespace
{
  // The one (and only one) system tick.
  volatile auto mcal_gpt_system_tick = mcal::gpt::value_type { };

  auto gpt_is_initialized() -> bool& __attribute__((used, noinline));

  auto gpt_is_initialized() -> bool&
  {
    static auto is_init = bool { };

    return is_init;
  }
}

extern "C"
{
  void mcal_gpt_init(void);

  void mcal_gpt_init(void) { mcal::gpt::init(nullptr); }
}

auto mcal::gpt::init(const config_type*) -> void
{
  if(!gpt_is_initialized())
  {
    // Set the is-initialized indication flag.
    gpt_is_initialized() = true;
  }
}

auto mcal::gpt::secure::get_time_elapsed() -> value_type
{
  return (gpt_is_initialized() ? SysTickTimer_ConsistentMicrosecondTick() : value_type { UINT8_C(0) });
}
