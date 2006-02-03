//#define _SCL_SECURE_NO_DEPRECATE
//#define BOOST_PROFILER_DISABLE
#include <boost/profiler.hpp>
//#include <iostream>

extern boost::profiler::detail::context ctx1;

int fact2(int n)
{
    BOOST_PROFILER_SET_CONTEXT(ctx1);
    //BOOST_PROFILER_SCOPE("factb()");
    BOOST_PROFILER_POINT(p1, "fact()");
    BOOST_PROFILER_START_P(p1);
    //int r = n > 1 ? n * fact2(n - 1) : 1;
    BOOST_PROFILER_STOP();
    return 0; 
    //return r;
}
