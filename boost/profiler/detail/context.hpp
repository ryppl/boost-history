#ifndef BOOST_PROFILER_DETAIL_CONTEXT_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_CONTEXT_HPP_INCLUDED

#include <boost/profiler/detail/semaphore.hpp>
#include <boost/profiler/detail/timer.hpp>
#include <vector>
#include <ostream>

namespace boost { namespace profiler { namespace detail
{

    struct entry;
    class point;
    class iterator;
    
    class context
    {
        friend class iterator;
        friend void start(point &, context &);
        friend void stop();
    public:
        context(const char *name);
        iterator begin() const;
        iterator end() const;
        double total_profiling_time() const;
        template<class Ch> void dump(std::basic_ostream<Ch> &stream) const;
    private:
        const char *m_name;
        tick_t m_earliest, m_latest;
        std::vector<entry> m_entries;
    };

    inline context::context(const char *name):
        m_name(name),
        m_earliest((std::numeric_limits<tick_t>::max)()),
        m_latest((std::numeric_limits<tick_t>::max)())
    {
    }

    inline double context::total_profiling_time() const
    {
        const timer_metrics &tm = get_timer_metrics();
        if (m_latest == (std::numeric_limits<tick_t>::max)())
            return 0;
        else
            return double(m_latest - m_earliest) / tm.ticks_per_second;
    }

    inline context &default_context()
    {
        static context ctx("default");
        return ctx;
    }

    inline context *&internal_current_context()
    {
        static context *ctx = &default_context();
        return ctx;
    }
    
    inline context &current_context()
    {
        return *internal_current_context();
    }

    inline void current_context(context &c)
    {
        scoped_semaphore sem(point_semaphore());
        internal_current_context() = &c;
    }

} } }

namespace boost { namespace profiler
{
    using detail::context;
    using detail::default_context;
    using detail::current_context;
} }

#endif
