#ifndef BOOST_PROFILER_DETAIL_CONTEXT_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_CONTEXT_HPP_INCLUDED

#include <boost/profiler/detail/semaphore.hpp>
#include <boost/profiler/detail/timer.hpp>
#include <list>
#include <vector>
#include <ostream>

namespace boost { namespace profiler { namespace detail
{

    struct entry;
    class point;
    class context;
    class iterator;
    
    inline std::list<context *> &contexts_list()
    {
        static std::list<context *> cl;
        return cl;
    }

    class context
    {
        friend class iterator;
        friend void start(point &, context &);
        friend void stop();
    public:
        context(const char *name);
        ~context();
        iterator begin() const;
        iterator end() const;
        const char *name() const;
        double total_profiling_time() const;
    private:
        const char *m_name;
        tick_t m_earliest, m_latest;
        std::vector<entry> m_entries;
        context(const context &);
        context &operator =(const context &);
    };

    inline context::context(const char *name):
        m_name(name),
        m_earliest((std::numeric_limits<tick_t>::max)()),
        m_latest((std::numeric_limits<tick_t>::max)())
    {
        scoped_semaphore sem(context_semaphore());
        std::list<context *> &cs = contexts_list();
        cs.push_back(this);
    }

    inline context::~context()
    {
        scoped_semaphore sem(context_semaphore());
        std::list<context *> &cl = contexts_list();
        cl.remove(this);
    }

    inline const char *context::name() const
    {
        return m_name;
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

#endif
