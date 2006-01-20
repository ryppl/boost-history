#ifndef BOOST_PROFILER_DETAIL_POINT_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_POINT_HPP_INCLUDED

#include <boost/profiler/detail/context.hpp>
#include <boost/profiler/detail/entry.hpp>
#include <boost/profiler/detail/timer.hpp>
#include <boost/profiler/detail/semaphore.hpp>
#include <boost/assert.hpp>
#include <vector>

namespace boost { namespace profiler { namespace detail
{

    class point;

    inline size_t next_index()
    {
        scoped_semaphore sem(index_semaphore());
        static size_t index;
        return index++;
    }

    inline std::vector<point *> &points_stack()
    {
        static std::vector<point *> stack;
        return stack;
    }

    class point
    {
        friend context;
    public:
        point(const char *name, const char *file, long line);
        void start(context &c);
        static void stop();
    private:
        const char *m_name;
        const char *m_file;
        long m_line;
        size_t m_index;
        long m_depth;
        context *m_context;
    };
    
    inline point::point(const char *name, const char *file, long line):
        m_name(name), m_file(file), m_line(line), 
        m_index(next_index()), m_depth(0), m_context(0)
    {
    }
    
    inline void point::start(context &c)
    {

        scoped_semaphore sem(point_semaphore());

        std::vector<point *> &stack = points_stack();

        ++m_depth;
        if (m_depth == 1)
        {
            m_context = &c;
            if (m_index >= c.m_entries.size())
                c.m_entries.resize(m_index + 1);
            entry &e = c.m_entries[m_index];
            e.p = this;
            tick_t t = ticks();
            e.start = t;
            e.start_exclusive = t;
            if (c.m_earliest == (std::numeric_limits<tick_t>::max)())
                c.m_earliest = t;
            if (!stack.empty())
            {
                point &p = *stack.back();
                entry &e = p.m_context->m_entries[p.m_index];
                e.total_exclusive += t - e.start_exclusive;
            }
        }

        stack.push_back(this);

    }

    inline void point::stop()
    {
        
        std::vector<point *> &stack = points_stack();
        BOOST_ASSERT(!stack.empty());
        
        scoped_semaphore sem(point_semaphore());
        
        point &p = *stack.back();
        stack.pop_back();
        BOOST_ASSERT(p.m_depth > 0);

        --p.m_depth;
        entry &e = p.m_context->m_entries[p.m_index];
        if (p.m_depth == 0)
        {
            tick_t t = ticks();
            e.total += t - e.start;
            e.total_exclusive += t - e.start_exclusive;
            p.m_context->m_latest = t;
            if (!stack.empty())
            {
                point &p = *stack.back();
                entry &e = p.m_context->m_entries[p.m_index];
                e.start_exclusive = t;
            }
        }
        ++e.count;
    }
    
} } }

#endif
