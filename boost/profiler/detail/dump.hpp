#ifndef BOOST_PROFILER_DETAIL_DUMP_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_DUMP_HPP_INCLUDED

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

    static const int c_ordinal = 0x1;
    static const int c_name = 0x2;
    static const int c_total = 0x4;
    static const int c_average_total = 0x8;
    static const int c_exclusive = 0x10;
    static const int c_average_exclusive = 0x20;
    static const int c_total_percent = 0x40;
    static const int c_exclusive_percent = 0x80;
    static const int c_exclusive_percent_sum = 0x100;
    static const int c_hitcount = 0x200;
    static const int c_location = 0x400;

    static const int c_full = c_ordinal | c_name | c_total | c_average_total | c_exclusive | c_average_exclusive | c_total_percent | c_exclusive_percent | c_exclusive_percent_sum | c_hitcount | c_location;
    static const int c_medium = c_ordinal | c_name | c_total | c_average_total | c_exclusive | c_average_exclusive | c_total_percent | c_exclusive_percent | c_exclusive_percent_sum | c_hitcount;
    static const int c_short = c_ordinal | c_name | c_total | c_average_total | c_exclusive | c_average_exclusive | c_total_percent | c_hitcount;
    static const int c_none = 0x0;

    class report_pred
    {
    public:
        report_pred(int sort_by):
            m_sort_by(sort_by)
        {
        }
        bool operator ()(const report &r1, const report &r2) const
        {
            switch (m_sort_by)
            {
                case c_name: return std::string(r1.name) < std::string(r2.name);
                case c_total: return r1.total_time > r2.total_time;
                case c_average_total: return r1.average_time > r2.average_time;
                case c_exclusive: return r1.total_exclusive_time > r2.total_exclusive_time;
                case c_average_exclusive: return r1.average_exclusive_time > r2.average_exclusive_time;
                case c_hitcount: return r1.hit_count > r2.hit_count;
                default: BOOST_ASSERT(0); return std::string(r1.name) < std::string(r2.name);
            }
        }
    private:
        int m_sort_by;
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
        if (u.first < 9.995)
            result = (boost::format("%0.2f%s") % u.first % u.second).str();
        else if (u.first < 99.95)
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

    inline std::string percentize(double x, unsigned width)
    {
        std::string result;
        x *= 100;
        if (x < 9.995)
            result = (boost::format("%0.2f%%") % x).str();
        else if (x < 99.95)
            result = (boost::format("%0.1f%%") % x).str();
        else
            result = (boost::format("%0.0f%%") % x).str();
        if (result.size() < width)
            return std::string(width - result.size(), ' ') + result;
        else
            return result;
    }

    inline void dump_system(std::ostream &stream)
    {
        const timer_metrics &tm = get_timer_metrics();
        double resolution = 1.0 / tm.ticks_per_second;
        double latency = 1.0 / tm.reports_per_second;
        double increment = double(tm.min_clock_step) / tm.ticks_per_second;
        boost::format fmt("Clock resolution:                    %s (%s)\n"
                          "Clock latency:                       %s\n"
                          "Smallest measured clock increment:   %s %s\n"
                          "Method used to obtain time:          %s\n"
                          "Method used to synchronize profiler: %s\n");
        fmt % stringize(resolution, "s/tick", 0, false)
            % stringize(double(tm.ticks_per_second), "Hz", 0, false)
            % stringize(latency, "s", 0, false)
            % stringize(increment, "s", 0, false)
            % (tm.clock_step_cpu_limited ? "(CPU bound)" : "(not CPU bound)")
            % detail::ticks_method_description
            % detail::semaphore_method_description;
        stream << fmt;
    }
    
    inline void dump(std::ostream &stream, 
                     const context &c, 
                     int columns = c_short, 
                     int sort_by = c_total)
    {
        
        std::vector<report> reports;
        {
            scoped_semaphore sem(point_semaphore());
            for (iterator it = c.begin(); it != c.end(); ++it)
                if (it->hit_count > 0)
                    reports.push_back(*it);
        }
        
        if (sort_by)
        {
            report_pred rp(sort_by);
            std::sort(reports.begin(), reports.end(), rp);
        }

        boost::format fmt("Profiling results for context \"%s\", profiling time %s\n\n");
        fmt % c.name() % stringize(c.total_profiling_time(), "s", 0, false);
        stream << fmt;
        
        if (!reports.empty())
        {
        
            size_t max_len = 6;
            BOOST_FOREACH(const report &r, reports)
                max_len = (std::max)(strlen(r.name), max_len);

            if (columns & c_ordinal)                stream << "   ##";
            if (columns & c_name)                   stream << std::string(max_len - 3, ' ') << "NAME";
            if (columns & c_total)                  stream << "   TOTAL";
            if (columns & c_average_total)          stream << "  AVGTOT";
            if (columns & c_exclusive)              stream << "   EXCLS";
            if (columns & c_average_exclusive)      stream << "  AVGEXC";
            if (columns & c_total_percent)          stream << " TOTAL%";
            if (columns & c_exclusive_percent)      stream << " EXCLS%";
            if (columns & c_exclusive_percent_sum)  stream << " EXSUM%";
            if (columns & c_hitcount)               stream << "  HITCOUNT";
            if (columns & c_location)               stream << "  LOCATION";
            stream << std::endl;
            
            if (columns & c_ordinal)                stream << "  ===";
            if (columns & c_name)                   stream << std::string(max_len - 3, '=') << "====";
            if (columns & c_total)                  stream << "========";
            if (columns & c_average_total)          stream << "========";
            if (columns & c_exclusive)              stream << "========";
            if (columns & c_average_exclusive)      stream << "========";
            if (columns & c_total_percent)          stream << "=======";
            if (columns & c_exclusive_percent)      stream << "=======";
            if (columns & c_exclusive_percent_sum)  stream << "=======";
            if (columns & c_hitcount)               stream << "==========";
            if (columns & c_location)               stream << "==========";
            stream << std::endl;

            size_t ordinal = 1;
            double total_exclusive_time_sum = 0;
            BOOST_FOREACH(const report &r, reports)
            {
                std::string name(r.name);
                if (name.size() < max_len)
                    name = std::string(max_len - name.size(), ' ') + name; 
                total_exclusive_time_sum += r.total_exclusive_time_fraction;
                if (columns & c_ordinal)                stream << boost::format("%4d.") % ordinal;
                if (columns & c_name)                   stream << " " << name;
                if (columns & c_total)                  stream << stringize(r.total_time, "s", 8, r.is_total_approx);
                if (columns & c_average_total)          stream << stringize(r.average_time, "s", 8, r.is_total_approx);
                if (columns & c_exclusive)              stream << stringize(r.total_exclusive_time, "s", 8, r.is_total_exclusive_approx);
                if (columns & c_average_exclusive)      stream << stringize(r.average_exclusive_time, "s", 8, r.is_total_exclusive_approx);
                if (columns & c_total_percent)          stream << percentize(r.total_time_fraction, 7);
                if (columns & c_exclusive_percent)      stream << percentize(r.total_exclusive_time_fraction, 7);
                if (columns & c_exclusive_percent_sum)  stream << percentize(total_exclusive_time_sum, 7);
                if (columns & c_hitcount)               stream << boost::format("%9dx") % r.hit_count;
                if (columns & c_location)               stream << boost::format("  %s(%d)") % r.file % r.line;
                stream << std::endl;
                ++ordinal;
            }
        }
        else
        {
            stream << "  No profiler entries recorded.\n";
        }

    }

    inline void dump(std::ostream &stream, 
                     int columns = c_short, 
                     int sort_by = c_total)
    {
        dump_system(stream);
        scoped_semaphore sem(context_semaphore());
        std::list<context *> &cl = contexts_list();
        BOOST_FOREACH(const context *c, cl)
            if (c->total_profiling_time() > 0)
            {
                stream << std::endl;
                dump(stream, *c, columns, sort_by);
            }
    }

} } }

#endif
