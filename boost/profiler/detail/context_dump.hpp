#ifndef BOOST_PROFILER_DETAIL_CONTEXT_DUMP_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_CONTEXT_DUMP_HPP_INCLUDED

#include <boost/profiler/detail/point.hpp>
#include <boost/profiler/detail/entry.hpp>
#include <boost/profiler/detail/context.hpp>
#include <boost/profiler/detail/timer.hpp>
#include <boost/profiler/detail/semaphore.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <limits>
#include <vector>
#include <ostream>

namespace boost { namespace profiler { namespace detail
{

    struct entry_pred
    {
        bool operator ()(const entry *e1, const entry *e2) const
        {
            return double(e1->total) / e1->count < double(e2->total) / e2->count;
        }
    };

    std::pair<double, std::string> unitize(double x, const std::string &unit)
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

    std::string stringize(double x, const std::string &unit, unsigned width, bool approx = false)
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
    void context::dump(std::basic_ostream<Ch> &stream)
    {
        
        const timer_metrics &tm = get_timer_metrics();
        
        scoped_semaphore sem(point_semaphore());
        
        std::vector<entry *> entries;
        BOOST_FOREACH(entry &e, m_entries)
            if (e.count > 0)         
                entries.push_back(&e);
        std::sort(entries.begin(), entries.end(), entry_pred());

        stream << "Profiling results for context \"" << m_name << "\":\n\n";
        
        double total = !entries.empty() ? double(m_latest - m_earliest) / tm.ticks_per_second : 0;
        double resolution = 1.0 / tm.ticks_per_second;
        double latency = 1.0 / tm.reports_per_second;
        double increment = double(tm.min_clock_step) / tm.ticks_per_second;
        boost::format fmt("  Total profiling time:     %s\n  Clock resolution:         %s (%s)\n  Clock latency:            %s\n  Smallest clock increment: %s %s\n\n");
        fmt % stringize(total, "s", 0)
            % stringize(resolution, "s/tick", 0)
            % stringize(double(tm.ticks_per_second), "Hz", 0)
            % stringize(latency, "s", 0)
            % stringize(increment, "s", 0)
            % (tm.clock_step_cpu_limited ? "(CPU bound)" : "(not CPU bound)");
        stream << fmt;

        if (entries.empty())
        {
            stream << "  No profiler entries recorded.\n";
        }
        else
        {
        
            const size_t min_name_length = 8;
            size_t maxlen = min_name_length;
            BOOST_FOREACH(entry *e, entries)
                maxlen = (std::max)(strlen(e->p->m_name), maxlen);

            stream << boost::format("   #%sName    Total      Avg      Exc   ExcAvg   Hitcount  Location\n") % std::string(maxlen - 2, ' ');
            stream << boost::format("  %s\n") % std::string(65 + maxlen, '=');

            int n = 1;
            BOOST_FOREACH(entry *e, entries)
            {
                
                BOOST_ASSERT(e->count > 0);
                
                std::string name(e->p->m_name);
                if (name.size() < maxlen)
                    name = std::string(maxlen - name.size(), ' ') + name; 
                
                double total = double(e->total) / tm.ticks_per_second;
                double avg = total / e->count;
                double total_exc = double(e->total_exclusive) / tm.ticks_per_second;
                double avg_exc = total_exc / e->count;
                
                double approx_threshold = 10.0 * (std::max)((std::max)(resolution, latency), increment);
                bool approx = avg < approx_threshold;
                bool approx_exc = avg_exc <  approx_threshold;

                boost::format fmt("%4d. %s  %s  %s  %s  %s %9dx  %s:%d");
                fmt % n 
                    % name 
                    % stringize(total, "s", 7, approx)
                    % stringize(avg, "s", 7, approx)
                    % stringize(total_exc, "s", 7, approx_exc)
                    % stringize(avg_exc, "s", 7, approx_exc)
                    % e->count
                    % e->p->m_file
                    % e->p->m_line;
                stream << fmt << "\n";
                ++n;
            }
        }

    }

    template<class Ch>
    std::basic_ostream<Ch> &operator <<(std::basic_ostream<Ch> &stream, context &c)
    {
        c.dump(stream);
        return stream;
    }

} } }

#endif
