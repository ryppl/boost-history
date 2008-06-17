// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/mutex.hpp>
#include <boost/dataflow/signals/component/timed_generator.hpp>
#include <boost/dataflow/signals/component/counter.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

int test_main(int, char* [])
{
    {
        //[ test_mutex_unfused
        signals::timed_generator<void ()> banger1;
        signals::timed_generator<void ()> banger2;
        signals::mutex<void ()> lock;
        signals::counter<void (), signals::output::unfused, volatile int> counter;
        
        banger1 >>= lock >>= counter;
        banger2 >>= lock;
        banger1.enable(0.5, 5);
        banger2.enable(0.5, 5);
        
        while (counter.count() < 10) {}
        int x = counter.count();
        BOOST_CHECK_EQUAL(x, 10);
        banger1.join();
        banger2.join();
        //]
    }
    {
        //[ test_mutex_fused
        signals::timed_generator<void (), signals::output::fused> banger1;
        signals::timed_generator<void (), signals::output::fused> banger2;
        signals::mutex<void (), signals::output::fused> lock;
        signals::counter<void (), signals::output::fused, volatile int> counter;

        banger1 >>= lock >>= counter;
        banger2 >>= lock;
        banger1.enable(0.5, 5);
        banger2.enable(0.5, 5);

        while (counter.count() < 10) {}
        int x = counter.count();
        BOOST_CHECK_EQUAL(x, 10);
        banger1.join();
        banger2.join();
        //]
    }
    return 0;
} // int test_main(int, char* [])
