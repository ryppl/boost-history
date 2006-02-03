#ifndef BOOST_PROFILER_DETAIL_MACROS_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_MACROS_HPP_INCLUDED

#include <boost/profiler/detail/context.hpp>
#include <boost/profiler/detail/point.hpp>
#include <boost/profiler/detail/startstop.hpp>

#if defined(BOOST_PROFILER_DISABLE)
    #define BOOST_PROFILER_POINT(Point, Name)
    #define BOOST_PROFILER_START_PC(Point, Context)
    #define BOOST_PROFILER_START_P(Point)
    #define BOOST_PROFILER_START_NC(Name, Context)
    #define BOOST_PROFILER_START_N(Name)
    #define BOOST_PROFILER_START()
    #define BOOST_PROFILER_STOP()
    #define BOOST_PROFILER_SCOPE_PC(Point, Context)
    #define BOOST_PROFILER_SCOPE_P(Point)
    #define BOOST_PROFILER_SCOPE_C(Name, Context)
    #define BOOST_PROFILER_SCOPE(Name)
    #define BOOST_PROFILER_CONTEXT(Context, Name)
    #define BOOST_PROFILER_EXTERN_CONTEXT(Context)
    #define BOOST_PROFILER_GET_CONTEXT()
    #define BOOST_PROFILER_SET_CONTEXT(Context)
    #define BOOST_PROFILER_GET_DEFAULT_CONTEXT()
    #define BOOST_PROFILER_DUMP(Dest)
    #define BOOST_PROFILER_DUMP_EX(Dest, Columns, OrderBy)
#else
    #define BOOST_PROFILER_POINT(Point, Name) static ::boost::profiler::detail::point Point(Name, __FILE__, __LINE__)
    #define BOOST_PROFILER_START_PC(Point, Context) ::boost::profiler::detail::start((Point), (Context))
    #define BOOST_PROFILER_START_P(Point) BOOST_PROFILER_START_PC((Point), ::boost::profiler::detail::current_context())
    #define BOOST_PROFILER_START_NC(Name, Context) { BOOST_PROFILER_POINT(boost_profiler_point_tmp, (Name)); BOOST_PROFILER_START_PC(boost_profiler_point_tmp, (Context)); }
    #define BOOST_PROFILER_START_N(Name) BOOST_PROFILER_START_NC((Name), ::boost::profiler::detail::current_context())
    #define BOOST_PROFILER_START() BOOST_PROFILER_START_N(BOOST_CURRENT_FUNCTION)
    #define BOOST_PROFILER_STOP() ::boost::profiler::detail::stop()
    #define BOOST_PROFILER_SCOPE_PC(Point, Context) ::boost::profiler::detail::scoped_profiler boost_profiler_scope_tmp((Point), (Context))
    #define BOOST_PROFILER_SCOPE_P(Point) BOOST_PROFILER_SCOPE_PC(Point, ::boost::profiler::detail::current_context())
    #define BOOST_PROFILER_SCOPE_NC(Name, Context) BOOST_PROFILER_POINT(boost_profiler_scope_point_tmp, (Name)); BOOST_PROFILER_SCOPE_PC(boost_profiler_scope_point_tmp, (Context))
    #define BOOST_PROFILER_SCOPE_N(Name) BOOST_PROFILER_SCOPE_NC((Name), ::boost::profiler::detail::current_context())
    #define BOOST_PROFILER_SCOPE() BOOST_PROFILER_SCOPE_N(BOOST_CURRENT_FUNCTION)
    #define BOOST_PROFILER_CONTEXT(Context, Name) ::boost::profiler::detail::context Context(Name)
    #define BOOST_PROFILER_EXTERN_CONTEXT(Context) extern ::boost::profiler::detail::context Context
    #define BOOST_PROFILER_GET_CONTEXT() ::boost::profiler::detail::current_context()
    #define BOOST_PROFILER_SET_CONTEXT(Context) ::boost::profiler::detail::current_context(Context)
    #define BOOST_PROFILER_GET_DEFAULT_CONTEXT() ::boost::profiler::detail::default_context()
    #define BOOST_PROFILER_DUMP_EX(Dest, Columns, OrderBy) ::boost::profiler::detail::dump_all_to_dest((Dest), (Columns), (OrderBy))
    #define BOOST_PROFILER_DUMP(Dest) BOOST_PROFILER_DUMP_EX((Dest), ::boost::profiler::c_short, ::boost::profiler::c_total)
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
