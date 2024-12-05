///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2024.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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

void UtilTimer_BlockingDelay(const uint64_t ms_count);

#if defined(__cplusplus)
}
#endif

#endif // UTIL_TIMER_2024_12_03_H
