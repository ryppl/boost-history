//  boost/chrono/lightweight_stopwatch.hpp  ------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_LIGHTWEIGHT_STOPWATCH__HPP
#define BOOST_CHRONO_LIGHTWEIGHT_STOPWATCH__HPP

#include <utility>

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/stopwatch_scoped.hpp>
#include <boost/system/error_code.hpp>
#include <boost/accumulators/framework/accumulator_set.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace chrono
  {

    template <typename Clock, typename Features, typename Weight>
    class lightweight_stopwatch_traits {
    public:
        static const bool is_accumulator_set = true;
        typedef accumulators::accumulator_set<typename Clock::duration::rep, Features, Weight> accumulator_set_t;
        typedef typename Clock::duration duration;
        static duration get_duration(accumulator_set_t& acc_) { return duration(accumulators::sum(acc_)); }
        static void set_duration(accumulator_set_t& acc_, duration d) { acc_(d.count()); }
        static void reset(accumulator_set_t& acc_) { acc_=accumulator_set_t(); }
    };

    template <typename Clock>
    class lightweight_stopwatch_traits<Clock, void, void> {
    public:
        static const bool is_accumulator_set = false;
        typedef typename Clock::duration duration;
        typedef duration accumulator_set_t;
        static duration get_duration(accumulator_set_t& acc_) { return acc_; }
        static void set_duration(accumulator_set_t& acc_, duration d) { acc_=d; }
        static void reset(accumulator_set_t& acc_) { acc_=duration(); }
    };

    //~ template <typename Clock>
    //~ class default_features;

    //~ template <>
    //~ class default_features<high_resolution_clock> {
        //~ typedef void type;
    //~ };

    struct dont_start_t{};
    static const dont_start_t dont_start = {};

    // forward declaration
    template <
        typename Clock=high_resolution_clock,
        typename Features=void,
        typename Weight=void
        //~ typename Features=typename default_features<Clock>::type
    >
    class lightweight_stopwatch;


//--------------------------------------------------------------------------------------//
    template <typename Clock, typename Features, typename Weight>
    class lightweight_stopwatch
    {
    public:
        typedef lightweight_stopwatch_traits<Clock,Features,Weight> traits;
        typedef typename traits::accumulator_set_t Accumulator;
        typedef Clock                       clock;
        typedef typename Clock::duration   duration;
        typedef typename Clock::time_point time_point;
        typedef typename Clock::rep        rep;
        typedef typename Clock::period     period;
        typedef Accumulator                 accumulator;
        static const bool is_accumulator_set = traits::is_accumulator_set;

        lightweight_stopwatch( accumulator& acc, system::error_code & ec = system::throws )
        : running_(false), suspended_(false),
          start_(duration::zero()), level_(0), partial_(duration::zero()), suspend_level_(0)
          , accumulated_(&acc), construction_(clock::now( ))
        {
            start(ec);
        }

        lightweight_stopwatch( accumulator& acc, const dont_start_t& )
        : running_(false), suspended_(false),
          start_(duration::zero()), level_(0), partial_(duration::zero()), suspend_level_(0)
          , accumulated_(&acc), construction_(clock::now( ))
        {
        }

        ~lightweight_stopwatch() {
            system::error_code ec;
            stop(ec);
        }

        
//--------------------------------------------------------------------------------------//
        std::pair<duration, time_point> restart( system::error_code & ec = system::throws )
        {
            time_point tmp=clock::now( ec );
            if (ec) return time_point();
            if (running_&&(level_==1)) {
                partial_ += tmp - start_;
                traits::set_duration(accumulated(),partial_);
                partial_=duration::zero();
            } else {
                running_=true;
            }
            start_=tmp;
            return std::make_pair(traits::get_duration(accumulated()),start_);
        }

        time_point start( system::error_code & ec = system::throws )
        {
            if (!running_) {
                time_point tmp = clock::now( ec );
                if (ec) return time_point();
                start_ = tmp;
                ++level_;
                running_ = true;
                return start_;
            } else {
                ++level_;
                ec.clear();
                return time_point();
            }
        }

        duration stop( system::error_code & ec = system::throws )
        {
            if (running_ && (--level_==0)) {
                time_point tmp=clock::now( ec );
                if (ec) return duration::zero();
                partial_ += tmp - start_;
                traits::set_duration(accumulated(),partial_);
                partial_=duration::zero();
                running_=false;
                return traits::get_duration(accumulated());
            } else {
                ec.clear();
                return duration::zero();
            }
        }

        duration suspend( system::error_code & ec = system::throws )
        {
            if (running_) {
                if (!suspended_) {
                    time_point tmp=clock::now( ec );
                    if (ec) return duration::zero();
                    ++suspend_level_;
                    partial_ += tmp - start_;
                    suspended_=true;
                    return traits::get_duration(accumulated());
                } else {
                    ++suspend_level_;
                    ec.clear();
                    return duration::zero();
                }
            } else {
                ec.clear();
                return duration::zero();
            }
        }

        time_point resume( system::error_code & ec = system::throws )
        {
            if (suspended_&&(--suspend_level_==0)) {
                time_point tmp = clock::now( ec );
                if (ec) return time_point();
                start_ = tmp;
                suspended_=false;
                return start_;
            } else {
                ec.clear();
                return time_point();
            }
        }

        duration elapsed( system::error_code & ec = system::throws )
        {
            if (running_) {
                if (suspended_)
                    return traits::get_duration(accumulated());
                else {
                    time_point tmp = clock::now( ec );
                    if (ec) return duration::zero();
                    return traits::get_duration(accumulated())+tmp - start_;
                }
            } else {
                return traits::get_duration(accumulated());
            }
        }

        time_point now( system::error_code & ec = system::throws )
        {
            return time_point(elapsed( ec ));
        }

        void reset( system::error_code & ec = system::throws )
        {
            construction_=clock::now( ec );
            if (ec) return;
            traits::reset(accumulated());
            running_=false;
            suspended_=false;
            partial_ = duration::zero();
            start_  = time_point(duration::zero());
            level_=0;
            suspend_level_=0;
        }

        accumulator& accumulated( )
        {
            return *accumulated_;
        }

        duration lifetime( system::error_code & ec = system::throws )
        {
            return  clock::now( ec ) - construction_;
        }

        typedef stopwatch_runner<lightweight_stopwatch<Clock,Features,Weight> > scoped_run;
        typedef stopwatch_stopper<lightweight_stopwatch<Clock,Features,Weight> > scoped_stop;
        typedef stopwatch_suspender<lightweight_stopwatch<Clock,Features,Weight> > scoped_suspend;
        typedef stopwatch_resumer<lightweight_stopwatch<Clock,Features,Weight> > scoped_resume;
    private:
        //~ lightweight_stopwatch operator=( lightweight_stopwatch const& rhs );
        //~ {
            //~ running_=rhs.running_; 
            //~ suspended_=rhs.suspended_;
            //~ start_=rhs.start_; 
            //~ level_=rhs.level_;
            //~ partial_=rhs.partial_;
            //~ suspend_level_=rhs.suspend_level_;
            //~ accumulated_=rhs.accumulated_;
            //~ construction_=rhs.construction_;
        //~ }
        
        bool running_;
        bool suspended_;
        time_point start_;
        std::size_t level_;
        duration partial_;
        std::size_t suspend_level_;
        accumulator* accumulated_;
        time_point construction_;
    };

//--------------------------------------------------------------------------------------//
    typedef accumulators::features<
                        accumulators::tag::count,
                        accumulators::tag::sum,
                        accumulators::tag::min,
                        accumulators::tag::max,
                        accumulators::tag::mean
        > default_features;
    typedef boost::chrono::lightweight_stopwatch< boost::chrono::system_clock > system_lightweight_stopwatch;
#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
    typedef boost::chrono::lightweight_stopwatch< boost::chrono::monotonic_clock > monotonic_lightweight_stopwatch;
#endif
    typedef boost::chrono::lightweight_stopwatch< boost::chrono::high_resolution_clock > high_resolution_lightweight_stopwatch;

    typedef boost::chrono::lightweight_stopwatch< boost::chrono::system_clock,default_features > system_lightweight_stopwatch_accumulator;
#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
    typedef boost::chrono::lightweight_stopwatch< boost::chrono::monotonic_clock,default_features > monotonic_lightweight_stopwatch_accumulator;
#endif
    typedef boost::chrono::lightweight_stopwatch< boost::chrono::high_resolution_clock,default_features > high_resolution_lightweight_stopwatch_accumulator;

//--------------------------------------------------------------------------------------//


  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
