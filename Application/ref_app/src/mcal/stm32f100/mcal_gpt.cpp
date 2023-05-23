///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_gpt.h>

extern "C"
{
  std::uint64_t mcal_gpt_system_tick;
}

auto mcal::gpt::init(const config_type*) -> void
{
}

auto mcal::gpt::secure::get_time_elapsed() -> value_type
{
  return mcal_gpt_system_tick;
}
