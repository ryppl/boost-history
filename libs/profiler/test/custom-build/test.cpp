#define _SCL_SECURE_NO_DEPRECATE
#include <boost/profiler.hpp>
#include <iostream>
#include <cmath>

namespace
{
    boost::profiler::context ctx1("ctx1");
    boost::profiler::context ctx2("ctx2");
}

inline void busy_delay(double seconds)
{
    using namespace boost::profiler;
    tick_t t = boost::profiler::ticks();
    const timer_metrics &tm = get_timer_metrics();
    tick_t n = t + static_cast<tick_t>(floor(seconds * tm.ticks_per_second + 0.5));
    while (boost::profiler::ticks() < n)
        ;
}

int fact(int n)
{
    BOOST_PROFILER_SCOPE("factb()");
    BOOST_PROFILER_POINT(p1, "fact()");
    BOOST_PROFILER_START_P(p1);
    int r = n > 1 ? n * fact(n - 1) : 1;
    BOOST_PROFILER_STOP();
    return r;
}

extern int fact2(int);

void delay(boost::profiler::tick_t d)
{
    boost::profiler::tick_t t = boost::profiler::ticks();
    while (boost::profiler::ticks() < t + d)
        ;
}

int main()
{
    fact2(10);
    fact(10);
    {
        BOOST_PROFILER_SCOPE("bd");     
        busy_delay(1);
        BOOST_PROFILER_START("bd2");
        busy_delay(1);
        BOOST_PROFILER_START("bd3");
        busy_delay(1);
        BOOST_PROFILER_STOP();
        busy_delay(1);
        BOOST_PROFILER_STOP();
    }
    boost::profiler::iterator b = boost::profiler::default_context().begin();
    boost::profiler::iterator e = boost::profiler::default_context().end();
    for (; b != e; ++b)
        std::cout << b->name << "\n";
    std::cout << boost::profiler::default_context();
}
