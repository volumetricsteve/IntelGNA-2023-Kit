/**
 @copyright Copyright (C) 2017-2022 Intel Corporation
 SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include <climits>
#include <cstdlib>
#include <cstdint>

#if defined(_WIN32)
#include <windows.h>
#endif

#include "profiler.h"

#if defined(_WIN32)

void getTsc(uint64_t * const result)
{
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER * const>(result));
}

#else
#if defined(__GNUC__) && !defined(__clang__)
static __inline__ uint64_t __rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return (static_cast<uint64_t>(lo)) | ((static_cast<uint64_t>(hi)) << 32);
}
#endif

void getTsc(uint64_t * const result)
{
    *result = __rdtsc();
}
#endif
