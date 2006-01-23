#define _SCL_SECURE_NO_DEPRECATE
#include <boost/profiler.hpp>
#include <iostream>

int fact2(int n)
{
    //BOOST_PROFILER_SCOPE("factb()");
    BOOST_PROFILER_POINT(p1, "fact()");
    BOOST_PROFILER_START_P(p1);
    //int r = n > 1 ? n * fact2(n - 1) : 1;
    BOOST_PROFILER_STOP();
    return 0; 
    //return r;
}
