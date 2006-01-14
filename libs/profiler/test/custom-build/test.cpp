#include <boost/foreach.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/profiler/profiler.hpp>
#include <iostream>

boost::profiler::context ctx1("ctx1");
boost::profiler::context ctx2("ctx2");

int fact(int n)
{
    BOOST_PROFILER_SCOPED("factb()");
    BOOST_PROFILER_POINT(p1, "fact()");
    BOOST_PROFILER_START(p1);
    int r = n > 1 ? n * fact(n - 1) : 1;
    BOOST_PROFILER_STOP(p1);
    return r;
}

void delay(boost::profiler::tick_t d)
{
    boost::profiler::tick_t t = boost::profiler::ticks();
    while (boost::profiler::ticks() < t + d)
        ;
}

char a[10000000];

int main()
{
    /*
    fact(10);
    for (int i = 0; i < 100; ++i)
    {
        PROFILER_BLOCK("loopb");
        PROFILER_POINT(p1, "loop");
        PROFILER_START(p1);
        if (i == 25)
            PROFILER_SET_CURRENT_CONTEXT(ctx1);
        if (i == 50)
            PROFILER_SET_CURRENT_CONTEXT(PROFILER_GET_DEFAULT_CONTEXT());
        delay(10);
        PROFILER_STOP(p1);
    }
    */

    {
        BOOST_PROFILER_SCOPED("whole");
        for (int i = 0; i < 10000; ++i)
        {
            a[i] = i;
            BOOST_PROFILER_SCOPED("loop");
        }
    }
    a[657] = 3;
    {
        BOOST_PROFILER_SCOPED("whole2");
        for (int i = 0; i < 10000; ++i)
        {
            a[i] = i;
        }
    }

    boost::property_tree::ptree pt;
    boost::profiler::default_context()->dump(pt);
    ctx1.dump(pt);
    ctx2.dump(pt);
    boost::property_tree::write_info(std::cout, pt);
}
