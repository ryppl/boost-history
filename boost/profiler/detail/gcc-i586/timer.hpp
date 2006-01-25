#ifndef BOOST_PROFILER_DETAIL_GCC_I586_TIMER_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_GCC_I586_TIMER_HPP_INCLUDED

#include <ctime>
#include <boost/cstdint.hpp>

namespace boost { namespace profiler { namespace detail 
{

    typedef boost::int64_t tick_t;
    static const char *ticks_method_description = "RDTSC instruction";
    
    inline tick_t ticks()
    {
        tick_t result;
        __asm__ __volatile__ ("rdtsc" : "=A"(result));
        return result;
    }

    inline tick_t ticks_per_second()
    {
        static tick_t result;
        if (result == 0)
        {
            clock_t t0 = std::clock(), t1, t2;
            do t1 = std::clock(); while (t1 <= t0);
            tick_t ticks1 = ticks();
            do t2 = std::clock(); while (t2 <= t1 + CLOCKS_PER_SEC / 4);
            tick_t ticks2 = ticks();
            result = (ticks2 - ticks1) * CLOCKS_PER_SEC / (t2 - t1);
        }
        return result;
    }

} } }

#endif

