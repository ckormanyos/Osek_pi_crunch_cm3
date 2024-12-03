#ifndef UTIL_TIMER_2024_12_03_H
#define UTIL_TIMER_2024_12_03_H

#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stdint.h>

#if defined(__cplusplus)
extern "C"
{
#endif

void TimerBlockingDelay(const uint64_t ms_count);

#if defined(__cplusplus)
}
#endif

#endif // UTIL_TIMER_2024_12_03_H
