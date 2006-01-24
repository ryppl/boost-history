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
#include <ctime>

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
            std::clock_t time1 = std::clock();
            tick_t ticks1 = ticks();
            while (std::clock() < time1 + CLOCKS_PER_SEC / 2)       // wait about 1/2 sec
                ;
            std::clock_t time2 = std::clock();
            tick_t ticks2 = ticks();
            result = (ticks2 - ticks1) * CLOCKS_PER_SEC / (time2 - time1);
        }
        return result;
    }

} } }

#endif

