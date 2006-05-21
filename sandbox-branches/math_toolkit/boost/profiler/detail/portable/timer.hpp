#ifndef BOOST_PROFILER_DETAIL_PORTABLE_TIMER_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_PORTABLE_TIMER_HPP_INCLUDED

#include <ctime>

namespace boost { namespace profiler { namespace detail 
{

    typedef std::clock_t tick_t;
    static const char *ticks_method_description = "std::clock()";
    
    inline tick_t ticks()
    {
        return std::clock();
    }

    inline tick_t ticks_per_second()
    {
        return CLOCKS_PER_SEC;
    }

} } }

#endif

