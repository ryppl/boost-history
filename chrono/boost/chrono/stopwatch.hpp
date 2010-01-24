//  boost/chrono/stopwatch.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_STOPWATCH_HPP
#define BOOST_CHRONO_STOPWATCH_HPP

#include <utility>
#include <boost/chrono/chrono.hpp>
#include <boost/chrono/stopwatch_scoped.hpp>
#include <boost/chrono/stopwatch_formatter.hpp>
#include <boost/chrono/stopwatch_reporter.hpp>
#include <boost/system/error_code.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace chrono
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
//~ When the freeze function is called while the watch is running, the elapsed time is frozen,
//~ allowing the elapsed time to that point to be read using the frozen function,
//~ but the watch mechanism continues running to record total elapsed time.
//--------------------------------------------------------------------------------------//

    template <class Clock=high_resolution_clock>
    class stopwatch;

    template <class Clock>
    struct stopwatch_reporter_default_formatter<stopwatch<Clock> > {
        typedef stopwatch_formatter type;
    };

    struct dont_start_t{};
    static const dont_start_t dont_start = {};
    template <class Clock>
    class stopwatch
    {
    public:
        typedef Clock                       clock;
        typedef typename Clock::duration    duration;
        typedef typename Clock::time_point  time_point;
        typedef typename Clock::rep         rep;
        typedef typename Clock::period      period;

        explicit stopwatch( system::error_code & ec = system::throws  )
        : running_(false), suspended_(false), start_(), level_(0), partial_(duration::zero()), suspend_level_(0)
        {
            start(ec);
        }

        explicit stopwatch( const dont_start_t& t )
        : running_(false), suspended_(false), start_(), level_(0), partial_(duration::zero()), suspend_level_(0)
        { }

        time_point start( system::error_code & ec = system::throws ) {
            ++level_;
            if (!running_) {
                time_point tmp = clock::now( ec );
                if (ec) return time_point();
                start_ = tmp;
                running_=true;
                return start_;
            } else {
                return time_point();
            }
        }

        duration stop( system::error_code & ec = system::throws ) {
            if (running_&&(--level_==0)) {
                time_point tmp=clock::now( ec );
                if (ec) return duration::zero();
                partial_ += tmp - start_;
                duration frozen= partial_;
                partial_=duration::zero();
                running_=false;
                return frozen;
            } else {
                return duration::zero();
            }
        }

        std::pair<duration, time_point> restart( system::error_code & ec = system::throws ) {
            duration frozen;
            time_point tmp=clock::now( ec );
            if (ec) return time_point();
            if (running_&&(--level_==0)) {
                partial_ += tmp - start_;
                frozen = partial_;
                partial_=duration::zero();
            } else {
                frozen = duration::zero();
                running_=true;
            }
            start_=tmp;
            ++level_;
            return std::make_pair(frozen, start_);
        }

        duration suspend( system::error_code & ec = system::throws ) {
            if (running_) {
                ++suspend_level_;
                if (!suspended_) {
                    time_point tmp=clock::now( ec );
                    if (ec) return duration::zero();
                    partial_ += tmp - start_;
                    suspended_=true;
                    return partial_;
                } else {
                    return duration::zero();
                }
            } else {
                return duration::zero();
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
            if (running_)
                return clock::now( ec ) - start_ + partial_;
            else
                return partial_;
        }

        time_point now( system::error_code & ec = system::throws )
        {
            return time_point(elapsed( ec ));
        }

        void reset( system::error_code & ec = system::throws ) {
            start_ = time_point();
            level_=0;
            running_=false;
        }

        typedef stopwatch_runner<stopwatch<Clock> > scoped_run;
        typedef stopwatch_stopper<stopwatch<Clock> > scoped_stop;
        typedef stopwatch_suspender<stopwatch<Clock> > scoped_suspend;
        typedef stopwatch_resumer<stopwatch<Clock> > scoped_resume;
        template <class Formatter=typename stopwatch_reporter_default_formatter<stopwatch<Clock> >::type> 
        struct get_reporter {
            typedef stopwatch_reporter<stopwatch<Clock>,Formatter > type;
        };
        typedef stopwatch_reporter<stopwatch<Clock> > reporter;

    private:
        bool running_;
        bool suspended_;
        time_point start_;
        std::size_t level_;
        duration partial_;
        std::size_t suspend_level_;

    };

//--------------------------------------------------------------------------------------//
    typedef boost::chrono::stopwatch< boost::chrono::system_clock > system_stopwatch;
#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
    typedef boost::chrono::stopwatch< boost::chrono::monotonic_clock > monotonic_stopwatch;
#endif
    typedef boost::chrono::stopwatch< boost::chrono::high_resolution_clock > high_resolution_stopwatch;



  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
