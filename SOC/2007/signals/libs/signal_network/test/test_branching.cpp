// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/storage.hpp>
#include <boost/signal_network/counter.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

int test_main(int, char* [])
{
    {
        //[ test_branching_unfused
        signet::storage<void ()>::unfused banger;
        signet::counter<void ()>::unfused counter;
        signet::storage<void (float)>::unfused floater(2.5f);
        signet::storage<void (float)>::unfused collector(0.0f);
        
        banger
            // floater connects to collector, banger to floater
            | (floater >>= collector)
            | counter; // and banger to counter
        
        banger();
        BOOST_CHECK_EQUAL(counter.count(), 1);
        BOOST_CHECK_EQUAL(collector.at<0>(), 2.5f);
        //]
    }
    {
        //[ test_branching_fused
        signet::storage<void ()> banger;
        signet::counter<void ()> counter;
        signet::storage<void (float)> floater(2.5f);
        signet::storage<void (float)> collector(0.0f);
        
        banger
            // floater connects to collector, banger to floater
            | (floater >>= collector)
            | counter; // and banger to counter
        
        banger();
        BOOST_CHECK_EQUAL(counter.count(), 1);
        BOOST_CHECK_EQUAL(collector.at<0>(), 2.5f);
        //]
    }
    return 0;
} // int test_main(int, char* [])

