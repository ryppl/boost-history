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
#include <boost/chrono/stopwatch_reporter.hpp>
#include <boost/chrono/stopwatch_accumulator_formatter.hpp>
#include <boost/system/error_code.hpp>
#include <boost/accumulators/framework/accumulator_set.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>

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
//~ A stopwatch can also also used to record split times or lap times.
//~ The elapsed time since the last start is available through the elapsed function.
//~ When the freeze function is called while the watch is running, the elapsed time is frozen,
//~ allowing the elapsed time to that point to be read using the frozen function,
//~ but the watch mechanism continues running to record total elapsed time.
//--------------------------------------------------------------------------------------//

    // forward declaration
    template <class Clock=high_resolution_clock,
        class Accumulator=accumulators::accumulator_set<typename Clock::duration::rep,
                    accumulators::features<
                        accumulators::tag::count,
                        accumulators::tag::sum,
                        accumulators::tag::min,
                        accumulators::tag::max,
                        accumulators::tag::mean >
        >
    >
    class stopwatch_accumulator;

    template <class Clock>
    struct stopwatch_reporter_default_formatter<stopwatch_accumulator<Clock> > {
        typedef stopwatch_accumulator_formatter type;
    };

//--------------------------------------------------------------------------------------//
    template <class Clock, class Accumulator>
    class stopwatch_accumulator
    {
    public:
        typedef Clock                       clock;
        typedef typename Clock::duration    duration;
        typedef typename Clock::time_point  time_point;
        typedef Accumulator accumulator;

        stopwatch_accumulator( )
        : running_(false), partial_(0)
        {
        }

        std::pair<duration, time_point>  restart( system::error_code & ec = system::throws ) {
            time_point tmp=clock::now( ec );
            if (ec) return time_point();
            if (running_&&(--level_==0)) {
                partial_ += tmp - start_;
                accumulated_(partial_.count());
                partial_=duration(0);
            } else {
                running_=true;
            }
            start_=tmp;
            ++level_;
            return std::make_pair(duration(accumulators::sum(accumulated_)),start_);
        }

        time_point start( system::error_code & ec = system::throws ) {
            ++level_;
            if (!running_) {
                time_point tmp = clock::now( ec );
                if (ec) return time_point();
                start_ = tmp;
                running_ = true;
                return start_;
            }  else {
                return time_point();
            }
        }

        duration stop( system::error_code & ec = system::throws ) {
            if (running_&&(--level_==0)) {
                time_point tmp=clock::now( ec );
                if (ec) return duration(0);
                partial_ += tmp - start_;
                accumulated_(partial_.count());
                partial_=duration(0);
                running_=false;
                return duration(accumulators::extract::sum(accumulated_));
            } else {
                return duration(0);
            }
        }

        duration suspend( system::error_code & ec = system::throws ) {
            if (running_) {
                ++suspend_level_;
                if (!suspended_) {
                    time_point tmp=clock::now( ec );
                    if (ec) return duration(0);
                    partial_ += tmp - start_;
                    suspended_=true;
                    return duration(accumulators::sum(accumulated_));
                } else {
                    return duration(0);
                }
            } else {
                return duration(0);
            }
        }
        time_point resume( system::error_code & ec = system::throws ) {
            if (suspended_&&(--suspend_level_==0)) {
                time_point tmp = clock::now( ec );
                if (ec) return time_point();
                start_ = tmp;
                suspended_=false;
                return start_;
            } else {
                return time_point();
            }
        }
        duration elapsed( system::error_code & ec = system::throws )
        {
            if (running_) {
                if (suspended_)
                    return duration(accumulators::sum(accumulated_));
                else {
                    time_point tmp = clock::now( ec );
                    if (ec) return duration(0);
                    return duration(accumulators::sum(accumulated_))+tmp - start_;
                }
            } else {
                return duration(accumulators::sum(accumulated_));
            }
        }

        void reset( ) {
            time_point tmp = time_point();
            start_  = tmp;
            accumulator tmp2;
            accumulated_ = tmp2;
        }
        accumulator& accumulated( ) { return accumulated_; }

        typedef stopwatch_runner<stopwatch_accumulator<Clock> > scoped_run;
        typedef stopwatch_suspender<stopwatch_accumulator<Clock> > scoped_suspend;
        typedef stopwatch_resumer<stopwatch_accumulator<Clock> > scoped_resume;
        typedef stopwatch_reporter<stopwatch_accumulator<Clock> > reporter;
    private:
        bool running_;
        bool suspended_;
        accumulator accumulated_;
        duration partial_;
        time_point start_;
        std::size_t level_;
        std::size_t suspend_level_;
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
