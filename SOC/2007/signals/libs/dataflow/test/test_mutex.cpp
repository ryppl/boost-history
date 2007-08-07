// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/component/mutex.hpp>
#include <boost/dataflow/signal/component/timed_generator.hpp>
#include <boost/dataflow/signal/component/counter.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;
using namespace boost::dataflow::operators;

int test_main(int, char* [])
{
    {
        //[ test_mutex_unfused
        signals::timed_generator<void (), signals::unfused> banger1;
        signals::timed_generator<void (), signals::unfused> banger2;
        signals::mutex<void (), signals::unfused> lock;
        signals::counter<void (), signals::unfused> counter;
        
        banger1 >>= lock >>= counter;
        banger2 >>= lock;
        banger1.enable(0.5, 5);
        banger2.enable(0.5, 5);
        
        while (counter.count() < 10) {}
        
        BOOST_CHECK_EQUAL(counter.count(), 10);
        banger1.join();
        banger2.join();
        //]
    }
    {
        //[ test_mutex_fused
        signals::timed_generator<void (), signals::fused> banger1;
        signals::timed_generator<void (), signals::fused> banger2;
        signals::mutex<void (), signals::fused> lock;
        signals::counter<void (), signals::fused> counter;

        banger1 >>= lock >>= counter;
        banger2 >>= lock;
        banger1.enable(0.5, 5);
        banger2.enable(0.5, 5);

        while (counter.count() < 10) {}

        BOOST_CHECK_EQUAL(counter.count(), 10);
        banger1.join();
        banger2.join();
        //]
    }
    return 0;
} // int test_main(int, char* [])
