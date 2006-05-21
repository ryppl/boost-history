#ifndef BOOST_PROFILER_DETAIL_STARTSTOP_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_STARTSTOP_HPP_INCLUDED

#include <boost/profiler/detail/context.hpp>
#include <boost/profiler/detail/entry.hpp>
#include <boost/profiler/detail/timer.hpp>
#include <boost/profiler/detail/semaphore.hpp>
#include <boost/assert.hpp>
#include <vector>

#include <iostream>

namespace boost { namespace profiler { namespace detail
{

    class point;
    class context;

    inline std::vector<point *> &points_stack()
    {
        static std::vector<point *> stack;
        return stack;
    }

    inline void start(point &p, context &c)
    {

        scoped_semaphore sem(point_semaphore());

        std::vector<point *> &stack = points_stack();

        ++p.m_depth;
        if (p.m_depth == 1)
        {
            p.m_context = &c;
            if (p.m_index >= c.m_entries.size())
                c.m_entries.resize(p.m_index + 1);
            entry &e = c.m_entries[p.m_index];
            e.p = &p;
            tick_t t = ticks();
            e.start = t;
            e.start_exclusive = t;
            if (c.m_earliest == (std::numeric_limits<tick_t>::max)())
                c.m_earliest = t;
            if (!stack.empty())
            {
                point &prev = *stack.back();
                entry &e = prev.m_context->m_entries[prev.m_index];
                e.total_exclusive += t - e.start_exclusive;
            }
        }

        stack.push_back(&p);

    }

    inline void stop()
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
        ++e.hit_count;
    }
    
} } }

#endif
