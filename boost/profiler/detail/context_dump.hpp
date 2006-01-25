#ifndef BOOST_PROFILER_DETAIL_CONTEXT_DUMP_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_CONTEXT_DUMP_HPP_INCLUDED

#include <boost/profiler/detail/report.hpp>
#include <boost/profiler/detail/context.hpp>
#include <boost/profiler/detail/iterator.hpp>
#include <boost/profiler/detail/timer.hpp>
#include <boost/profiler/detail/semaphore.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <limits>
#include <vector>
#include <ostream>

namespace boost { namespace profiler { namespace detail
{

    struct report_pred
    {
        bool operator ()(const report &r1, const report &r2) const
        {
            return r1.total_time > r2.total_time;
        }
    };

    inline std::pair<double, std::string> unitize(double x, const std::string &unit)
    {
        if (x < 999.5e-18)
            return std::make_pair(0.0, unit);
        if (x < 999.5e-15)
            return std::make_pair(x * 1e+15, "f" + unit);
        if (x < 999.5e-12)
            return std::make_pair(x * 1e+12, "p" + unit);
        else if (x < 999.5e-9)
            return std::make_pair(x * 1e+9, "n" + unit);
        else if (x < 999.5e-6)
            return std::make_pair(x * 1e+6, "u" + unit);
        else if (x < 999.5e-3)
            return std::make_pair(x * 1e+3, "m" + unit);
        else if (x < 999.5)
            return std::make_pair(x, unit);
        else if (x < 999.5e+3)
            return std::make_pair(x / 1e+3, "k" + unit);
        else if (x < 999.5e+6)
            return std::make_pair(x / 1e+6, "M" + unit);
        else if (x < 999.5e+9)
            return std::make_pair(x / 1e+9, "G" + unit);
        else if (x < 999.5e+12)
            return std::make_pair(x / 1e+12, "E" + unit);
        else if (x < 999.5e+15)
            return std::make_pair(x / 1e+15, "P" + unit);
        else
            return std::make_pair(x, unit);
    }

    inline std::string stringize(double x, const std::string &unit, unsigned width, bool approx)
    {
        std::pair<double, std::string> u = unitize(x, unit);
        std::string result;
        if (u.first < 9.5)
            result = (boost::format("%0.2f%s") % u.first % u.second).str();
        else if (u.first < 99.5)
            result = (boost::format("%0.1f%s") % u.first % u.second).str();
        else
            result = (boost::format("%0.0f%s") % u.first % u.second).str();
        if (approx)
            result = '~' + result;
        if (result.size() < width)
            return std::string(width - result.size(), ' ') + result;
        else
            return result;
    }

    template<class Ch> 
    inline void context::dump(std::basic_ostream<Ch> &stream) const
    {
        
        const timer_metrics &tm = get_timer_metrics();
        
        scoped_semaphore sem(point_semaphore());
        
        std::vector<report> reports;
        for (iterator it = begin(); it != end(); ++it)
            if (it->hit_count > 0)
                reports.push_back(*it);
        std::sort(reports.begin(), reports.end(), report_pred());

        stream << "Profiling results for context \"" << m_name << "\":\n\n";
        
        double total = !reports.empty() ? double(m_latest - m_earliest) / tm.ticks_per_second : 0;
        double resolution = 1.0 / tm.ticks_per_second;
        double latency = 1.0 / tm.reports_per_second;
        double increment = double(tm.min_clock_step) / tm.ticks_per_second;
        boost::format fmt("  Total profiling time:                %s\n"
                          "  Clock resolution:                    %s (%s)\n"
                          "  Clock latency:                       %s\n"
                          "  Smallest measured clock increment:   %s %s\n"
                          "  Method used to obtain time:          %s\n"
                          "  Method used to synchronize profiler: %s\n\n");
        fmt % stringize(total, "s", 0, false)
            % stringize(resolution, "s/tick", 0, false)
            % stringize(double(tm.ticks_per_second), "Hz", 0, false)
            % stringize(latency, "s", 0, false)
            % stringize(increment, "s", 0, false)
            % (tm.clock_step_cpu_limited ? "(CPU bound)" : "(not CPU bound)")
            % detail::ticks_method_description
            % detail::semaphore_method_description;
        stream << fmt;

        if (reports.empty())
        {
            stream << "  No profiler entries recorded.\n";
        }
        else
        {
        
            size_t max_len = 6;
            BOOST_FOREACH(const report &r, reports)
                max_len = (std::max)(strlen(r.name), max_len);

            stream << boost::format("   #%sName    Total      Avg      Exc   ExcAvg  Total%%    Exc%% ExcSum%%  Hitcount  Location\n") % std::string(max_len - 2, ' ');
            stream << boost::format("  %s\n") % std::string(80 + max_len, '=');

            size_t index = 1;
            double total_exclusive_time_sum = 0;
            BOOST_FOREACH(const report &r, reports)
            {
                
                std::string name(r.name);
                if (name.size() < max_len)
                    name = std::string(max_len - name.size(), ' ') + name; 
                
                double approx_threshold = 10.0 * (std::max)((std::max)(resolution, latency), increment);
                bool approx = r.average_time < approx_threshold;
                bool approx_exc = r.average_exclusive_time <  approx_threshold;

                total_exclusive_time_sum += r.total_exclusive_time_fraction;

                boost::format fmt("%4d. %s  %s  %s  %s  %s  %5.1f%%  %5.1f%%  %5.1f%% %9dx  %s(%d)");
                fmt % index 
                    % name 
                    % stringize(r.total_time, "s", 7, approx)
                    % stringize(r.average_time, "s", 7, approx)
                    % stringize(r.total_exclusive_time, "s", 7, approx_exc)
                    % stringize(r.average_exclusive_time, "s", 7, approx_exc)
                    % (r.total_time_fraction * 100)
                    % (r.total_exclusive_time_fraction * 100)
                    % (total_exclusive_time_sum * 100)
                    % r.hit_count
                    % r.file
                    % r.line;
                stream << fmt << "\n";
                ++index;
            }
        }

    }

    template<class Ch>
    inline std::basic_ostream<Ch> &operator <<(std::basic_ostream<Ch> &stream, context &c)
    {
        c.dump(stream);
        return stream;
    }

} } }

#endif
