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
    
    class context
    {
        friend point;
    public:    
        context(const char *name);
        template<class Ch> void dump(std::basic_ostream<Ch> &stream);
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
