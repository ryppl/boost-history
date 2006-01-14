#ifndef BOOST_PROFILER_DETAIL_TIMER_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_TIMER_HPP_INCLUDED

#include <boost/config.hpp>

#if defined(BOOST_WINDOWS) && defined(BOOST_MSVC)
    #include <boost/profiler/detail/msvc/timer.hpp>
#else
    #include <boost/profiler/detail/portable/timer.hpp>
#endif

namespace boost { namespace profiler 
{
    using detail::tick_t;
    using detail::ticks;
    using detail::ticks_per_second;
} }

#endif

