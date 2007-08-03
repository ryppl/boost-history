// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/support.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

//[ test_connect

struct receiver
{
    typedef void result_type;
    typedef boost::dataflow::signal_consumer consumer_category;
    
    receiver() : stored(0) {}
    
    void operator()(int x)
    {
        stored = x;
    }
    
    int stored;
};

int test_main(int, char* [])
{

    boost::signal<void(int)> p;
    receiver r;
    
    boost::dataflow::connect(p, r);
    
    p(3);
    
    BOOST_CHECK_EQUAL(r.stored, 3);

    return 0;
}

//]
