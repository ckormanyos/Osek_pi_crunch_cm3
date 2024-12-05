///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2024.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <util/Util/UtilTimer.h>
#include <util/utility/util_time.h>

extern "C"
void UtilTimer_BlockingDelay(const uint64_t ms_count)
{
  using local_timer_type = util::timer<std::uint64_t>;
  using local_tick_type  = typename local_timer_type::tick_type;

  local_timer_type::blocking_delay(local_timer_type::milliseconds(local_tick_type { ms_count }));
}
