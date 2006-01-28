#ifndef BOOST_PROFILER_DETAIL_REPORT_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_REPORT_HPP_INCLUDED

#include <boost/profiler/detail/entry.hpp>
#include <boost/profiler/detail/point.hpp>
#include <boost/profiler/detail/timer.hpp>
#include <boost/cstdint.hpp>

namespace boost { namespace profiler { namespace detail
{

    struct report
    {
        report(const entry &e, double total_profiling_time);
        const char *name;
        const char *file;
        long line;
        double total_time;
        double total_exclusive_time;
        double average_time;
        double average_exclusive_time;
        double total_time_fraction;
        double total_exclusive_time_fraction;
        boost::uint64_t hit_count;
        bool is_total_approx;
        bool is_total_exclusive_approx;
    };

    inline report::report(const entry &e, double total_profiling_time)
    {
        
        const timer_metrics &tm = get_timer_metrics();
        
        double resolution = 1.0 / tm.ticks_per_second;
        double latency = 1.0 / tm.reports_per_second;
        double increment = double(tm.min_clock_step) / tm.ticks_per_second;
        double approx_threshold = 10.0 * (std::max)((std::max)(resolution, latency), increment);
        
        name = e.p->m_name;
        file = e.p->m_file;
        line = e.p->m_line;
        total_time = double(e.total) / tm.ticks_per_second;
        total_exclusive_time = double(e.total_exclusive) / tm.ticks_per_second;
        average_time = double(e.total) / tm.ticks_per_second / e.count;
        average_exclusive_time = double(e.total_exclusive) / tm.ticks_per_second / e.count;
        total_time_fraction = (total_profiling_time > 0) ? total_time / total_profiling_time : 0;
        total_exclusive_time_fraction = (total_profiling_time > 0) ? total_exclusive_time / total_profiling_time : 0;
        hit_count = e.count;
        is_total_approx = average_time < approx_threshold;
        is_total_exclusive_approx = average_exclusive_time <  approx_threshold;

    }
    
} } }

namespace boost { namespace profiler
{
    using detail::report;
} }

#endif
