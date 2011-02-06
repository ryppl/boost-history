//  boost/stopwatches/stopwatch.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_STOPWATCHES_STOPWATCH_HPP
#define BOOST_STOPWATCHES_STOPWATCH_HPP

#include <utility>
#include <boost/chrono/chrono.hpp>
#include <boost/stopwatches/stopwatch_scoped.hpp>
#include <boost/stopwatches/lightweight_stopwatch.hpp>
#include <boost/system/error_code.hpp>
#include <boost/utility/base_from_member.hpp>

namespace boost
{
  namespace stopwatches
  {

//--------------------------------------------------------------------------------------//
//                                    stopwatch
//
//~ A stopwatch is a class designed to measure the amount of time elapsed from a particular time
//~ when activated to when it is deactivated.

//~ Calling start starts the timer running, and calling stop stops it.
//~ A call to reset resets the stopwatch to zero.
//~ A stopwatch can also also used to record split times or lap times.
//~ The elapsed time since the last start is available through the elapsed function.
//--------------------------------------------------------------------------------------//

    //~ template <class Clock=chrono::high_resolution_clock>
    template <class Clock=chrono::system_clock>
    class stopwatch;

    //~ struct dont_start_t{};
    //~ static const dont_start_t dont_start = {};
//--------------------------------------------------------------------------------------//

    template <class Clock>
    class stopwatch : private base_from_member<typename Clock::duration>, public lightweight_stopwatch<Clock>
    {
    public:
        typedef base_from_member<typename Clock::duration> pbase_type;
        explicit stopwatch( system::error_code & ec = BOOST_CHRONO_THROWS  )
        : pbase_type(), lightweight_stopwatch<Clock>(pbase_type::member, ec)
        {
        }

        explicit stopwatch( const dont_start_t& t )
        : pbase_type(), lightweight_stopwatch<Clock>(pbase_type::member, t)
        { }
        
    };

//--------------------------------------------------------------------------------------//
    typedef boost::stopwatches::stopwatch< boost::chrono::system_clock > system_stopwatch;
#ifdef BOOST_CHRONO_HAS_CLOCK_STEADY
    typedef boost::stopwatches::stopwatch< boost::chrono::steady_clock > steady_stopwatch;
#endif
    typedef boost::stopwatches::stopwatch< boost::chrono::high_resolution_clock > high_resolution_stopwatch;



  } // namespace stopwatches
} // namespace boost

#endif
