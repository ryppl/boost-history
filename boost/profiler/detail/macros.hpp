#ifndef BOOST_PROFILER_DETAIL_MACROS_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_MACROS_HPP_INCLUDED

#if defined(BOOST_PROFILER_DISABLE)
    
    #define BOOST_PROFILER_POINT(Point, Name)
    #define BOOST_PROFILER_START_PC(Point, Context)
    #define BOOST_PROFILER_START_P(Point)
    #define BOOST_PROFILER_START_C(Name, Context)
    #define BOOST_PROFILER_START(Name)
    #define BOOST_PROFILER_START_FUNCTION()
    #define BOOST_PROFILER_STOP()
    #define BOOST_PROFILER_SCOPE_PC(Point, Context)
    #define BOOST_PROFILER_SCOPE_P(Point)
    #define BOOST_PROFILER_SCOPE_C(Name, Context)
    #define BOOST_PROFILER_SCOPE(Name)
    #define BOOST_PROFILER_FUNCTION()
    #define BOOST_PROFILER_CONTEXT(Context, Name)
    #define BOOST_PROFILER_EXTERN_CONTEXT(Context)
    #define BOOST_PROFILER_GET_CONTEXT()
    #define BOOST_PROFILER_SET_CONTEXT(Context)
    #define BOOST_PROFILER_GET_DEFAULT_CONTEXT()
    #define BOOST_PROFILER_DUMP_CONTEXT_EX(Context, Dest, Columns, OrderBy, Limit)
    #define BOOST_PROFILER_DUMP_CONTEXT(Context, Dest)
    #define BOOST_PROFILER_DUMP_ALL_EX(Dest, Columns, OrderBy, Limit)
    #define BOOST_PROFILER_DUMP_ALL(Dest)
    #define BOOST_PROFILER_DUMP_SYSTEM(Dest)

#else

    #include <boost/profiler/detail/context.hpp>
    #include <boost/profiler/detail/point.hpp>
    #include <boost/profiler/detail/startstop.hpp>

    #define BOOST_PROFILER_POINT(Point, Name) static ::boost::profiler::detail::point Point(Name, __FILE__, __LINE__)
    #define BOOST_PROFILER_START_PC(Point, Context) ::boost::profiler::detail::start((Point), (Context))
    #define BOOST_PROFILER_START_P(Point) BOOST_PROFILER_START_PC((Point), ::boost::profiler::detail::current_context())
    #define BOOST_PROFILER_START_C(Name, Context) { BOOST_PROFILER_POINT(boost_profiler_point_tmp, (Name)); BOOST_PROFILER_START_PC(boost_profiler_point_tmp, (Context)); }
    #define BOOST_PROFILER_START(Name) BOOST_PROFILER_START_C((Name), ::boost::profiler::detail::current_context())
    #define BOOST_PROFILER_START_FUNCTION() BOOST_PROFILER_START(BOOST_CURRENT_FUNCTION)
    #define BOOST_PROFILER_STOP() ::boost::profiler::detail::stop()
    #define BOOST_PROFILER_SCOPE_PC(Point, Context) ::boost::profiler::detail::scoped_profiler boost_profiler_scope_tmp((Point), (Context))
    #define BOOST_PROFILER_SCOPE_P(Point) BOOST_PROFILER_SCOPE_PC(Point, ::boost::profiler::detail::current_context())
    #define BOOST_PROFILER_SCOPE_C(Name, Context) BOOST_PROFILER_POINT(boost_profiler_scope_point_tmp, (Name)); BOOST_PROFILER_SCOPE_PC(boost_profiler_scope_point_tmp, (Context))
    #define BOOST_PROFILER_SCOPE(Name) BOOST_PROFILER_SCOPE_C((Name), ::boost::profiler::detail::current_context())
    #define BOOST_PROFILER_FUNCTION() BOOST_PROFILER_SCOPE(BOOST_CURRENT_FUNCTION)
    #define BOOST_PROFILER_CONTEXT(Context, Name) ::boost::profiler::detail::context Context(Name)
    #define BOOST_PROFILER_EXTERN_CONTEXT(Context) extern ::boost::profiler::detail::context Context
    #define BOOST_PROFILER_GET_CONTEXT() ::boost::profiler::detail::current_context()
    #define BOOST_PROFILER_SET_CONTEXT(Context) ::boost::profiler::detail::current_context(Context)
    #define BOOST_PROFILER_GET_DEFAULT_CONTEXT() ::boost::profiler::detail::default_context()
    #define BOOST_PROFILER_DUMP_CONTEXT_EX(Context, Dest, Columns, OrderBy, Limit) ::boost::profiler::detail::dump_context_to_dest((Context), (Dest), (Columns), (OrderBy), (Limit))
    #define BOOST_PROFILER_DUMP_CONTEXT(Context, Dest) BOOST_PROFILER_DUMP_CONTEXT_EX((Context), (Dest), ::boost::profiler::c_full, ::boost::profiler::c_total, 0)
    #define BOOST_PROFILER_DUMP_ALL_EX(Dest, Columns, OrderBy, Limit) ::boost::profiler::detail::dump_all_to_dest((Dest), (Columns), (OrderBy), (Limit))
    #define BOOST_PROFILER_DUMP_ALL(Dest) BOOST_PROFILER_DUMP_ALL_EX((Dest), ::boost::profiler::c_full, ::boost::profiler::c_total, 0)
    #define BOOST_PROFILER_DUMP_SYSTEM(Dest) ::boost::profiler::detail::dump_system_to_dest(Dest)

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

#endif
