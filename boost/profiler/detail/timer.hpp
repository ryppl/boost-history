#ifndef BOOST_PROFILER_DETAIL_TIMER_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_TIMER_HPP_INCLUDED

#include <boost/config.hpp>
#include <algorithm>
#include <limits>

#if defined(BOOST_WINDOWS) && defined(BOOST_MSVC)
    #include <boost/profiler/detail/msvc/timer.hpp>
#else
    #include <boost/profiler/detail/portable/timer.hpp>
#endif

namespace boost { namespace profiler { namespace detail
{

    struct timer_metrics
    {
        tick_t ticks_per_second;
        tick_t reports_per_second;
        tick_t min_clock_step;
        bool clock_step_cpu_limited;
    };
    
    const timer_metrics &get_timer_metrics()
    {
        static bool done;
        static timer_metrics tm;
        if (!done)
        {

            tm.ticks_per_second = ticks_per_second();

            {
                tick_t count = 0;
                tick_t b = ticks();
                while (ticks() - b < tm.ticks_per_second / 8)
                    ++count;
                tm.reports_per_second = 8 * count;
            }
            
            {
                tm.min_clock_step = (std::numeric_limits<tick_t>::max)();
                tm.clock_step_cpu_limited = true;
                tick_t t, b = ticks(), p = b;
                do
                {
                    t = ticks();
                    if (t > p) 
                        tm.min_clock_step = (std::min)(tm.min_clock_step, t - p);
                    else
                        tm.clock_step_cpu_limited = false;
                    p = t;
                } while (t - b < tm.ticks_per_second / 8);
            }

            done = true;

        }
        
        return tm;

    }
    
} } }

namespace boost { namespace profiler 
{
    using detail::tick_t;
    using detail::ticks;
    using detail::timer_metrics;
    using detail::get_timer_metrics;
} }

#endif

