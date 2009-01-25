//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_TEST_DATA_TYPES__HPP
#define BOOST_INTERTHREADS_TEST_DATA_TYPES__HPP


#include <boost/thread/xtime.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test;

namespace
{
inline boost::xtime delay(int secs, int msecs=0, int nsecs=0)
{
    const int MILLISECONDS_PER_SECOND = 1000;
    const int NANOSECONDS_PER_SECOND = 1000000000;
    const int NANOSECONDS_PER_MILLISECOND = 1000000;

    boost::xtime xt;
    if (boost::TIME_UTC != boost::xtime_get (&xt, boost::TIME_UTC))
        BOOST_ERROR ("boost::xtime_get != boost::TIME_UTC");

    nsecs += xt.nsec;
    msecs += nsecs / NANOSECONDS_PER_MILLISECOND;
    secs += msecs / MILLISECONDS_PER_SECOND;
    nsecs += (msecs % MILLISECONDS_PER_SECOND) * NANOSECONDS_PER_MILLISECOND;
    xt.nsec = nsecs % NANOSECONDS_PER_SECOND;
    xt.sec += secs + (nsecs / NANOSECONDS_PER_SECOND);

    return xt;
}

void sleep(int sec)
{
	boost::xtime t;
	boost::xtime_get(&t,1);	
    t.sec += sec; 
    boost::thread::sleep(t);
}
}



struct non_copyable_functor
    :    boost::noncopyable
{
    unsigned value;
    typedef unsigned result_type;
    
    non_copyable_functor():
        value(0)
    {}
    
    unsigned operator()()
    {
        value=999;
        return value;
    }
};

struct copyable_functor
{
    unsigned value;
    typedef int result_type;
    
    copyable_functor():
        value(0)
    {}
    
    int operator()()
    {
        value=999;
        return value;
    }
};

struct print_xml
{
    template <typename T>
    void operator()(T const& x) const
    {
        std::cout
//            << '<' << typeid(x).name() << '>'
            << x
//            << "</" << typeid(x).name() << '>'
            << std::endl
            ;
    }
};

#endif
