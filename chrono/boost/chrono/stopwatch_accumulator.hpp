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

namespace boost {  namespace chrono {

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

namespace kind {
struct idle {};
struct running {};
struct running_and_idle {};
}
namespace detail {
template <
    class Clock,
    class RunningIdleKind,
    class Accumulator
>
struct basic_stopwatch_accumulator;
    
template <
    class Clock,
    class Accumulator
>
class basic_stopwatch_accumulator<Clock, kind::running, Accumulator> {
public:
    typedef Clock                       clock;
    typedef typename Clock::duration    duration;
    typedef typename Clock::time_point  time_point;
    typedef Accumulator accumulator;

    accumulator& accumulated( ) { return accumulated_; }   
  
protected:
    basic_stopwatch_accumulator( )
    : running_(false), suspended_(false), accumulated_(), 
      partial_(duration::zero()), start_((duration::zero())), level_(0), suspend_level_(0)
    {}

    void idle_on_start(time_point &) {}
    void idle_on_stop(time_point &) {}
    
    bool running_;
    bool suspended_;
    accumulator accumulated_;
    duration partial_;
    time_point start_;
    std::size_t level_;
    std::size_t suspend_level_;
    
};

        
template <
    class Clock,
    class Accumulator
>
class basic_stopwatch_accumulator<Clock, kind::running_and_idle, Accumulator> 
    : public basic_stopwatch_accumulator<Clock, kind::running, Accumulator>
{
        typedef basic_stopwatch_accumulator<Clock, kind::running, Accumulator> base_type;
public:
    typedef Clock                       clock;
    typedef typename Clock::duration    duration;
    typedef typename Clock::time_point  time_point;
    typedef Accumulator accumulator;

    accumulator& idle_accumulated( ) { return idle_accumulated_; }   
    
protected:
    basic_stopwatch_accumulator( )
    : base_type(),
      stopped_(false), idle_accumulated_(), idle_partial_(duration::zero()), stop_(duration::zero()) 
    {}

    void idle_on_start(time_point &tmp) {
        if (stopped_) {
            idle_partial_ += tmp - this->start_;
            idle_accumulated_(idle_partial_.count());
            idle_partial_=duration::zero();
        }
    }

    void idle_on_stop(time_point &tmp) {
        stop_ = tmp;
        stopped_=true;
    }

    void idle_on_reset() {
        stopped_=false;
        idle_accumulated_ = accumulator();
        idle_partial_=duration::zero();
        stop_  = time_point(duration::zero());
    }
    
    bool stopped_;
    accumulator idle_accumulated_;
    duration idle_partial_;
    time_point stop_;
};

}        


    // forward declaration
    template <class Clock=high_resolution_clock,
        class RunningIdleKind=kind::running,
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
    template <class Clock, class RunningIdleKind, class Accumulator >
    class stopwatch_accumulator 
        : public detail::basic_stopwatch_accumulator<Clock, RunningIdleKind, Accumulator>
    {
        typedef detail::basic_stopwatch_accumulator<Clock, RunningIdleKind, Accumulator> base_type;
    public:
        typedef Clock                       clock;
        typedef typename Clock::duration    duration;
        typedef typename Clock::time_point  time_point;
        typedef Accumulator accumulator;

        stopwatch_accumulator( ) : base_type() {}

        std::pair<duration, time_point>  restart( system::error_code & ec = system::throws ) {
            time_point tmp=clock::now( ec );
            if (ec) return time_point();
            if (this->running_&&(--this->level_==0)) {
                this->partial_ += tmp - this->start_;
                this->accumulated_(this->partial_.count());
                this->partial_=duration::zero();
            } else {
                this->running_=true;
            }
            this->start_=tmp;
            ++this->level_;
            return std::make_pair(duration(accumulators::sum(this->accumulated_)),this->start_);
        }

        time_point start( system::error_code & ec = system::throws ) {
            if (!this->running_) {
                time_point tmp = clock::now( ec );
                if (ec) return time_point();
                ++this->level_;
                this->start_ = tmp;
                this->running_ = true;
                this->idle_on_start(tmp);
                return this->start_;
            }  else {
                ++this->level_;
                return time_point();
            }
        }

        duration stop( system::error_code & ec = system::throws ) {
            if (this->running_) {
                if (this->level_==1) {
                    time_point tmp=clock::now( ec );
                    if (ec) return duration::zero();
                    --this->level_;
                    this->partial_ += tmp - this->start_;
                    this->accumulated_(this->partial_.count());
                    this->partial_=duration::zero();
                    this->running_=false;
                    this->idle_on_stop(tmp);
                    return duration(accumulators::extract::sum(this->accumulated_));
                } else {
                   --this->level_; 
                   return duration::zero();
                }
            } else {
                return duration::zero();
            }
        }

        duration suspend( system::error_code & ec = system::throws ) {
            if (this->running_) {
                ++this->suspend_level_;
                if (!this->suspended_) {
                    time_point tmp=clock::now( ec );
                    if (ec) return duration::zero();
                    this->partial_ += tmp - this->start_;
                    this->suspended_=true;
                    return duration(accumulators::sum(this->accumulated_));
                } else {
                    return duration::zero();
                }
            } else {
                return duration::zero();
            }
        }
        time_point resume( system::error_code & ec = system::throws ) {
            if (this->suspended_&&(--this->suspend_level_==0)) {
                time_point tmp = clock::now( ec );
                if (ec) return time_point();
                this->start_ = tmp;
                this->suspended_=false;
                return this->start_;
            } else {
                return time_point();
            }
        }
        duration elapsed( system::error_code & ec = system::throws )
        {
            if (this->running_) {
                if (this->suspended_)
                    return duration(accumulators::sum(this->accumulated_));
                else {
                    time_point tmp = clock::now( ec );
                    if (ec) return duration::zero();
                    return duration(accumulators::sum(this->accumulated_))+tmp - this->start_;
                }
            } else {
                return duration(accumulators::sum(this->accumulated_));
            }
        }

        void reset( ) {
            this->running_=false;
            this->suspended_=false;
            this->accumulated_ = accumulator();
            this->partial_=duration::zero();
            this->start_  = time_point(duration::zero());
            this->level_=0;
            this->suspend_level_=0;
            this->idle_on_reset();
        }

        typedef stopwatch_runner<stopwatch_accumulator<Clock> > scoped_run;
        typedef stopwatch_stopper<stopwatch_accumulator<Clock> > scoped_stop;
        typedef stopwatch_suspender<stopwatch_accumulator<Clock> > scoped_suspend;
        typedef stopwatch_resumer<stopwatch_accumulator<Clock> > scoped_resume;
        typedef stopwatch_reporter<stopwatch_accumulator<Clock> > reporter;
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
