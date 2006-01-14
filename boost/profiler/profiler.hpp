#ifndef BOOST_PROFILER_HPP_INCLUDED
#define BOOST_PROFILER_HPP_INCLUDED

#include <boost/profiler/detail/timer.hpp>
#include <boost/profiler/detail/scoped_semaphore.hpp>
#include <boost/property_tree/ptree.hpp>
#include <vector>

#if defined(BOOST_PROFILER_DISABLE)
    #define BOOST_PROFILER_START_CTX(Point, Context) static_cast<void>(0)
    #define BOOST_PROFILER_POINT(Point, Name) static_cast<void>(0)
    #define BOOST_PROFILER_START(Point) static_cast<void>(0)
    #define BOOST_PROFILER_STOP(Point) static_cast<void>(0)
    #define BOOST_PROFILER_SCOPED_CTX(Name, Context) static_cast<void>(0)
    #define BOOST_PROFILER_SCOPED(Name) static_cast<void>(0)
#else
    #define BOOST_PROFILER_START_CTX(Point, Context) (Point).start(Context)
    #define BOOST_PROFILER_POINT(Point, Name) static boost::profiler::point Point(Name, __FILE__, __LINE__)
    #define BOOST_PROFILER_START(Point) BOOST_PROFILER_START_CTX(Point, *boost::profiler::current_context())
    #define BOOST_PROFILER_STOP(Point) (Point).stop()
    #define BOOST_PROFILER_SCOPED_CTX(Name, Context) BOOST_PROFILER_POINT(tmp, Name); boost::profiler::detail::scoped_profiler boost_profiler_tmp(tmp, Context)
    #define BOOST_PROFILER_SCOPED(Name) BOOST_PROFILER_SCOPED_CTX(Name, *boost::profiler::current_context())
#endif

namespace boost { namespace profiler { namespace detail
{

    inline semaphore_t *index_semaphore()
    {
        static semaphore_t semaphore = 1;
        return &semaphore;
    }

    inline semaphore_t *point_semaphore()
    {
        static semaphore_t semaphore = 1;
        return &semaphore;
    }
    
    inline int next_index()
    {
        scoped_semaphore sem(index_semaphore());
        static int index;
        return index++;
    }

} } }

namespace boost { namespace profiler
{

    class point;
    class context;
    struct entry;

    class point
    {
        friend context;
    public:
        point(const char *name, const char *file, long line);
        void start(context &c);
        void stop();
    private:
        const char *m_name;
        const char *m_file;
        long m_line;
        unsigned m_index;
        context *m_context;
        unsigned m_depth;
    };
    
    struct entry
    {
        tick_t start;
        tick_t total;
        unsigned long count;
        const point *p;
        entry(): 
            total(0), count(0), p(0)
        {
        }
    };
    
    class context
    {
        friend point;
    public:    
        context(const char *name);
        template<class Ptree> void dump(Ptree &pt);
    private:
        const char *m_name;
        std::vector<entry> m_entries;
    };

    inline point::point(const char *name, const char *file, long line):
        m_name(name), m_file(file), m_line(line), 
        m_index(detail::next_index()), m_depth(0), m_context(0)
    {
    }
    
    inline void point::start(context &c)
    {
        detail::scoped_semaphore sem(detail::point_semaphore());
        ++m_depth;
        if (m_depth == 1)
        {
            m_context = &c;
            if (m_index >= c.m_entries.size())
            {
                c.m_entries.resize(m_index + 1);
                c.m_entries[m_index].p = this;
            }
            entry &e = c.m_entries[m_index];
            e.start = ticks();
        }
    }
    
    inline void point::stop()
    {
        detail::scoped_semaphore sem(detail::point_semaphore());
        BOOST_ASSERT(m_depth > 0);
        --m_depth;
        entry &e = m_context->m_entries[m_index];
        if (m_depth == 0)
            e.total += ticks() - e.start;
        ++e.count;
    }

    inline context::context(const char *name):
        m_name(name)
    {
    }

    template<class Ptree>
    inline void context::dump(Ptree &pt)
    {
        tick_t tps = ticks_per_second();
        detail::scoped_semaphore sem(detail::point_semaphore());
        Ptree &root = *pt.put(m_name, "");
        for (std::vector<entry>::const_iterator it = m_entries.begin(); it != m_entries.end(); ++it)
            if (it->p)
            {
                double total = double(it->total) / tps;
                double average = it->count ? total / it->count : 0.0;
                Ptree &child = *root.put(it->p->m_name, "");
                child.put("file", it->p->m_file);
                child.put("line", it->p->m_line);
                child.put("total", total);
                child.put("average", average);
                child.put("count", it->count);
            }
    }

    inline context *default_context()
    {
        static context default_context("default");
        return &default_context;
    }

    inline context *&current_context()
    {
        static context *current = default_context();
        return current;
    }

} }

namespace boost { namespace profiler { namespace detail
{

    class scoped_profiler
    {
    public:
        scoped_profiler(point &p, context &c):
            m_point(p)
        {
            BOOST_PROFILER_START_CTX(m_point, c);
        }
        ~scoped_profiler()
        {
            BOOST_PROFILER_STOP(m_point);
        }
    private:
        point &m_point;
        scoped_profiler(const scoped_profiler &);
        scoped_profiler &operator =(const scoped_profiler &);
    };

} } }

#endif
