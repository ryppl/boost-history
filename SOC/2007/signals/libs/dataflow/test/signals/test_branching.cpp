// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/component/counter.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

int test_main(int, char* [])
{
    {
        //[ test_branching_unfused
        signals::storage<void ()> banger;
        signals::counter<void ()> counter;
        signals::storage<void (float)> floater(2.5f);
        signals::storage<void (float)> collector(0.0f);
        
        
        // ---Connect the dataflow network -----------------------------
        //
        // ,--------.  void()          ,---------.
        // | banger | -------+-------> | counter |
        // `--------'        |         `---------'
        //                   |
        //                   V
        //            ,-(send_slot)-.  void(float)   ,-----------.
        //            |   floater   | -------------> | collector |
        //            `-------------'                `-----------'
        //
        // -------------------------------------------------------------
 
        banger
            | counter
            | floater.send_slot();
        floater >>= collector;
        
        banger();
        BOOST_CHECK_EQUAL(counter.count(), 1);
        BOOST_CHECK_EQUAL(collector.at<0>(), 2.5f);
        //]
    }
    {
        //[ test_branching_fused
        signals::storage<void (), signals::output::fused> banger;
        signals::counter<void (), signals::output::fused> counter;
        signals::storage<void (float), signals::output::fused> floater(2.5f);
        signals::storage<void (float), signals::output::fused> collector(0.0f);
        
        banger
            | counter
            | floater.send_slot();
        floater >>= collector;

        banger();
        BOOST_CHECK_EQUAL(counter.count(), 1);
        BOOST_CHECK_EQUAL(collector.at<0>(), 2.5f);
        //]
    }
    return 0;
} // int test_main(int, char* [])

