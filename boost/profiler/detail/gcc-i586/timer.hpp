// ----------------------------------------------------------------------------
// Copyright (C) 2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROFILER_DETAIL_GCC_I586_TIMER_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_GCC_I586_TIMER_HPP_INCLUDED

#include <boost/cstdint.hpp>
#include <sys/time.h>
#include <ctime>

#ifndef BOOST_HAS_GETTIMEOFDAY
    #error gettimeofday() required
#endif

namespace boost { namespace profiler { namespace detail 
{

    typedef boost::int64_t tick_t;
    
    inline tick_t ticks()
    {
        tick_t result;
        __asm__ __volatile__ ("rdtsc" : "=A" (result));
        return result;
    }

    inline tick_t ticks_per_second()
    {
        static tick_t result;
        if (result == 0)
        {
            timeval tv1, tv2;
            gettimeofday(&tv1, 0);
            tick_t ticks1 = ticks();
            clock_t time = clock();
            while (clock() <= time + CLOCKS_PER_SEC / 8)
                ;
            gettimeofday(&tv2, 0);
            tick_t ticks2 = ticks();
            time_t dsec =  tv2.tv_sec - tv1.tv_sec;
            suseconds_t dusec = tv2.tv_usec - tv1.tv_usec;
            double dt = double(dsec) + double(dusec) / 1000000.0;
            result = static_cast<tick_t>((ticks2 - ticks1) / dt);
        }
        return result;
    }

} } }

#endif

