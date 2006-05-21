#ifndef BOOST_PROFILER_DETAIL_POINT_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_POINT_HPP_INCLUDED

#include <cstddef>

namespace boost { namespace profiler { namespace detail
{

    class context;
    struct report;

    inline size_t next_index()
    {
        scoped_semaphore sem(index_semaphore());
        static size_t index;
        return index++;
    }

    class point
    {
        friend class context;
        friend struct report;
        friend void start(point &, context &);
        friend void stop();
    public:
        point(const char *name, const char *file, long line);
    private:
        const char *m_name;
        const char *m_file;
        long m_line;
        size_t m_index;
        size_t m_depth;
        context *m_context;
        point(const point &);
        point &operator =(const point &);
    };
    
    inline point::point(const char *name, const char *file, long line):
        m_name(name), m_file(file), m_line(line), 
        m_index(next_index()), m_depth(0), m_context(0)
    {
    }
    
} } }

#endif
