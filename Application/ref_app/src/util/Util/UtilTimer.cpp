#include <util/Util/UtilTimer.h>
#include <util/utility/util_time.h>

namespace local
{
  using timer_type = util::timer<std::uint64_t>;
}

extern "C"
void TimerBlockingDelay(const uint64_t ms_count)
{
  for(unsigned j = 0U; j < static_cast<unsigned>(ms_count); ++j)
  {
    for(volatile unsigned loop = 0U; loop < 2000U; ++loop)
    {
      asm volatile("nop");
    }
  }
}
