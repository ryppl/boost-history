#ifndef BOOST_PROFILER_DETAIL_MACROS_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_MACROS_HPP_INCLUDED

#include <boost/profiler/detail/context.hpp>
#include <boost/profiler/detail/point.hpp>
#include <boost/profiler/detail/startstop.hpp>

#if defined(BOOST_PROFILER_DISABLE)
    #define BOOST_PROFILER_POINT(Point, Name) static_cast<void>(0)
    #define BOOST_PROFILER_START_PC(Point, Context) static_cast<void>(0)
    #define BOOST_PROFILER_START_P(Point) static_cast<void>(0)
    #define BOOST_PROFILER_START_C(Name, Context) static_cast<void>(0)
    #define BOOST_PROFILER_START(Name) static_cast<void>(0)
    #define BOOST_PROFILER_STOP() static_cast<void>(0)
    #define BOOST_PROFILER_SCOPE_PC(Point, Context) static_cast<void>(0)
    #define BOOST_PROFILER_SCOPE_P(Point) static_cast<void>(0)
    #define BOOST_PROFILER_SCOPE_C(Name, Context) static_cast<void>(0)
    #define BOOST_PROFILER_SCOPE(Name) static_cast<void>(0)
#else
    #define BOOST_PROFILER_POINT(Point, Name) static ::boost::profiler::detail::point Point(Name, __FILE__, __LINE__)
    #define BOOST_PROFILER_START_PC(Point, Context) ::boost::profiler::detail::start((Point), (Context))
    #define BOOST_PROFILER_START_P(Point) BOOST_PROFILER_START_PC((Point), ::boost::profiler::current_context())
    #define BOOST_PROFILER_START_C(Name, Context) { BOOST_PROFILER_POINT(boost_profiler_point_tmp, (Name)); BOOST_PROFILER_START_PC(boost_profiler_point_tmp, (Context)); }
    #define BOOST_PROFILER_START(Name) BOOST_PROFILER_START_C((Name), ::boost::profiler::current_context())
    #define BOOST_PROFILER_STOP() ::boost::profiler::detail::stop()
    #define BOOST_PROFILER_SCOPE_PC(Point, Context) ::boost::profiler::detail::scoped_profiler boost_profiler_scope_tmp((Point), (Context))
    #define BOOST_PROFILER_SCOPE_P(Point) BOOST_PROFILER_SCOPE_PC(Point, ::boost::profiler::current_context())
    #define BOOST_PROFILER_SCOPE_C(Name, Context) BOOST_PROFILER_POINT(boost_profiler_scope_point_tmp, (Name)); BOOST_PROFILER_SCOPE_PC(boost_profiler_scope_point_tmp, (Context))
    #define BOOST_PROFILER_SCOPE(Name) BOOST_PROFILER_SCOPE_C((Name), ::boost::profiler::current_context())
#endif

namespace boost { namespace profiler { namespace detail
{

    class scoped_profiler
    {
    public:
        scoped_profiler(point &p, context &c)
        {
            BOOST_PROFILER_START_PC(p, c);
        }
        ~scoped_profiler()
        {
            BOOST_PROFILER_STOP();
        }
    private:
        scoped_profiler(const scoped_profiler &);
        scoped_profiler &operator =(const scoped_profiler &);
    };

} } }

#endif
