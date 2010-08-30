//  boost/chrono/stopwatch_accumulator.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_STOPWATCH_ACCUMULATOR_HPP
#define BOOST_CHRONO_STOPWATCH_ACCUMULATOR_HPP

#include <utility>

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/stopwatch_scoped.hpp>
#include <boost/system/error_code.hpp>
#include <boost/chrono/lightweight_stopwatch.hpp>
#include <boost/utility/base_from_member.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace chrono
  {

//--------------------------------------------------------------------------------------//
//                                    stopwatch
//
//~ A stopwatch accumulator is a class designed to measure the amount of time elapsed from a particular time
//~ when activated to when it is deactivated.

//~ Calling start starts the timer running, and calling stop stops it.
//~ A call to reset resets the stopwatch to zero.
//~ A stopwatch can also be used to record split times or lap times.
//~ The elapsed time since the last start is available through the elapsed function.
//--------------------------------------------------------------------------------------//

    // forward declaration
    template <class Clock=high_resolution_clock,
        typename Features=accumulators::features<
                        accumulators::tag::count,
                        accumulators::tag::sum,
                        accumulators::tag::min,
                        accumulators::tag::max,
                        accumulators::tag::mean >,
        typename Weight=void
    >
    class stopwatch_accumulator;


//--------------------------------------------------------------------------------------//

    template <class Clock, typename Features, typename Weight>
    class stopwatch_accumulator
        : private base_from_member<typename accumulators::accumulator_set<typename Clock::duration::rep, Features, Weight> >,
          public lightweight_stopwatch<Clock,Features,Weight>
    {
    public:
        typedef base_from_member<typename accumulators::accumulator_set<typename Clock::duration::rep, Features, Weight> > pbase_type;

        stopwatch_accumulator( )
        : pbase_type(), lightweight_stopwatch<Clock,Features,Weight>(pbase_type::member, dont_start)
        { }
    //~ private:
        //~ stopwatch_accumulator operator=( stopwatch_accumulator const& rhs );
    };

//--------------------------------------------------------------------------------------//
    typedef boost::chrono::stopwatch_accumulator< boost::chrono::system_clock > system_stopwatch_accumulator;
#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
    typedef boost::chrono::stopwatch_accumulator< boost::chrono::monotonic_clock > monotonic_stopwatch_accumulator;
#endif
    typedef boost::chrono::stopwatch_accumulator< boost::chrono::high_resolution_clock > high_resolution_stopwatch_accumulator;

//--------------------------------------------------------------------------------------//


  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
