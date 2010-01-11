//  boost/chrono/timer.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_TIMER_ACCUMULATOR_HPP
#define BOOST_CHRONO_TIMER_ACCUMULATOR_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/system/error_code.hpp>

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

    struct dont_start_t{};
    static const dont_start_t dont_start = {};
    template <class Clock=high_resolution_clock>
    class stopwatch
    {
    public:
        typedef Clock                       clock;
        typedef typename Clock::duration    duration;
        typedef typename Clock::time_point  time_point;

        explicit stopwatch( system::error_code & ec = system::throws  )
        : running_(false), frozen_(0), start_(0), level_(0)
        {
            start(ec);
        }

        explicit stopwatch( const dont_start_t& t )
        : running_(false), frozen_(0), start_(0), level_(0)
        { }

        time_point start( system::error_code & ec = system::throws ) {
            ++level_;
            if (!running_) {
                time_point tmp = clock::now( ec );
                if (ec) return time_point(0);
                start_ = tmp;
                running_=true;
                return start_;
            } else {
                return time_point(0);
            }
        }

        duration stop( system::error_code & ec = system::throws ) {
            if (running_&&(--level_==0)) {
                duration tmp = elapsed(ec);
                if (ec) return duration(0);
                frozen_ = tmp;
                running_=false;
                return frozen_;
            } else {
                return duration(0);
            }
        }

        time_point restart( system::error_code & ec = system::throws ) {
            time_point tmp=clock::now( ec );
            if (ec) return time_point(0);
            if (running_&&(--level_==0)) {
                frozen_ =  (tmp - start_);
            } else {
                running_=true;
            }
            start_=tmp;
            ++level_;
            return start_;
        }

        duration elapsed( system::error_code & ec = system::throws )
        {
            if (running_) {
                return clock::now( ec ) - start_;
            } else {
                return frozen_;
            }
        }

        void reset( system::error_code & ec = system::throws ) {
            start_ = time_point(0);
            frozen_=0;
            level_=0;
            running_=false;
        }
        duration accumulated( ) { return 0; }

    private:
        bool running_;
        duration frozen_;
        time_point start_;
        std::size_t level_;
    };

//--------------------------------------------------------------------------------------//
    template <class Clock=high_resolution_clock>
    class stopwatch_accumulator
    {
    public:
        typedef Clock                       clock;
        typedef typename Clock::duration    duration;
        typedef typename Clock::time_point  time_point;

        explicit stopwatch_accumulator( )
        : running_(false), duration_(0), start_(0), times_(0)
        { }

        time_point restart( system::error_code & ec = system::throws ) {
            time_point tmp=clock::now( ec );
            if (ec) return time_point(0);
            if (running_&&(--level_==0)) {
                accumulated_ +=  (tmp - start_);
            } else {
                running_=true;
            }
            start_=tmp;
            ++times_;
            ++level_;
            return start_;
        }

        time_point start( system::error_code & ec = system::throws ) {
            ++level_;
            if (!running_) {
                time_point tmp = clock::now( ec );
                if (ec) return time_point(0);
                start_ = tmp;
                running_ = true;
                ++times_;
                return start_;
            }  else {
                return duration(0);
            }
        }

        duration stop( system::error_code & ec = system::throws ) {
            if (running_&&(--level_==0)) {
                duration tmp = elapsed(ec);
                if (ec) return duration(0);
                accumulated_ += tmp;
                running_=false;
                return accumulated_;
            } else return duration(0);
        }

        duration suspend( system::error_code & ec = system::throws ) {
            if (running_) {
                ++suspend_level_;
                if (!suspended_) {
                    duration tmp = elapsed(ec);
                    if (ec) return duration(0);
                    accumulated_ += tmp;
                    suspended_=true;
                    return accumulated_;
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
                if (ec) return time_point(0);
                start_ = tmp;
                suspended_=false;
                return start;
            } else {
                return time_point(0);
            }
        }
        duration elapsed( system::error_code & ec = system::throws )
        {
            if (running_) {
                if (suspended_)
                    return accumulated_;
                else {
                    time_point tmp = clock::now( ec );
                    if (ec) return 0;
                    return accumulated_ + tmp - start_;
            } else {
                return accumulated_;
            }
        }

        std::size_t times() const {
            return times_
        }
        duration average() const {
            return (times_?accumulated_/times_:0;
        }

        void reset( system::error_code & ec = system::throws ) {
            time_point tmp = time_point(ec);
            if (ec) return 0;
            start_  = tmp;
            times_=0;
            accumulated_=0;
        }
        duration accumulated( ) { return accumulated; }

    private:
        bool running_;
        bool suspended_;
        duration accumulated_;
        time_point start_;
        std::size_t level_;
        std::size_t suspend_level_;
        std::size_t times_;
    };

//--------------------------------------------------------------------------------------//
    typedef boost::chrono::timer< boost::chrono::system_clock > system_timer;
#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
    typedef boost::chrono::timer< boost::chrono::monotonic_clock > monotonic_timer;
#endif
    typedef boost::chrono::timer< boost::chrono::high_resolution_clock > high_resolution_timer;

//--------------------------------------------------------------------------------------//
    template <class Stopwatch> class stropwatch_enabler {
    public:
        typedef Stopwatch stopwatch_type;
        stropwatch_enabler(stopwatch_type & a, system::error_code & ec = system::throws) 
        : stopwatch_(a) {
            stopwatch_.start(ec);
        }
        ~stropwatch_enabler() {
            system::error_code & ec;
            stopwatch_.stop(ec);
        }
        duration elapsed(system::error_code & ec = system::throws)
        {
            return stopwatch_.elapsed(ec)-stopwatch_.accumulated();
        }
    private:
        stopwatch_type& stopwatch_;
        stropwatch_enabler();//= delete;
        stropwatch_enabler(const enable_timer_accumulation&); // = delete;
        stropwatch_enabler& operator=(const enable_timer_accumulation&); // = delete;

    };

//--------------------------------------------------------------------------------------//
    template <class Stopwatch> class stropwatch_suspender {
    public:
        typedef Stopwatch stopwatch_type;
        stropwatch_suspender(stopwatch_type & a, system::error_code & ec = system::throws)  
        : stopwatch_(a) {
            acc_.suspend(ec);
        }
        ~stropwatch_suspender() {
            system::error_code & ec;
            acc_.resume(ec);
        }
    private:
        stopwatch_type& stopwatch_;
        stropwatch_suspender(); // = delete;
        stropwatch_suspender(const stropwatch_suspender&); // = delete;
        stropwatch_suspender& operator=(const stropwatch_suspender&); // = delete;
    }

//--------------------------------------------------------------------------------------//
    template <class Stopwatch> class stropwatch_resumer {
    public:
        typedef Stopwatch stopwatch_type;
        stropwatch_resumer(stopwatch_type & a, system::error_code & ec = system::throws)  
        : stopwatch_(a) {
            acc_.resume(ec);
        }
        ~stropwatch_resumer() {
            system::error_code & ec;
            acc_.suspend(ec);
        }
    private:
        stopwatch_type& stopwatch_;
        stropwatch_resumer(); // = delete;
        stropwatch_resumer(const stropwatch_resumer&); // = delete;
        stropwatch_resumer& operator=(const stropwatch_resumer&); // = delete;
    }
    
    
  } // namespace chrono
} // namespace boost

#endif
