// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/mutex.hpp>
#include <boost/signal_network/timed_generator.hpp>
#include <boost/signal_network/counter.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

#include <boost/test/included/test_exec_monitor.hpp>

int test_main(int, char* [])
{
    {
        //[ test_mutex_unfused
        signet::timed_generator<void ()>::unfused banger1;
        signet::timed_generator<void ()>::unfused banger2;
        signet::mutex<void ()>::unfused lock;
        signet::counter<void ()>::unfused counter;
        
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
        signet::timed_generator<void ()> banger1;
        signet::timed_generator<void ()> banger2;
        signet::mutex<void ()> lock;
        signet::counter<void ()> counter;

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
