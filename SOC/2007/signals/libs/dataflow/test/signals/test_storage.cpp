// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

int test_main(int, char* [])
{
    {
        //[ test_storage_unfused
        // instantiate all of the components we need
        signals::storage<void ()> banger;
        signals::storage<void (float)> floater(2.5f);
        signals::storage<void (float)> collector(0.0f);
                
        // ---Connect the dataflow network -----------------------------
        //
        // ,--------.  void()   
        // | banger | -------. 
        // `--------'        |
        //                   |
        //                   V
        //            ,-(send_slot)-.  void(float)   ,-----------.
        //            |   floater   | -------------> | collector |
        //            `-------------'                `-----------'
        //
        // -------------------------------------------------------------
        
        banger >>= floater.send_slot();
        floater >>= collector;

        // signal from banger is will invoke floater.send(), which causes
        // floater to output 2.5
        banger();
        BOOST_CHECK_EQUAL(floater.at<0>(), 2.5f);
        BOOST_CHECK_EQUAL(collector.at<0>(), 2.5f);

        floater.close();
        floater(1.5f); // change the value in floater
        invoke(floater); // we can also signal floater directly
        BOOST_CHECK_EQUAL(collector.at<0>(), 1.5f);
    //]
    }

    {
        //[ test_storage_fused
        // instantiate all of the components we need
        signals::storage<void (), signals::output::fused> banger;
        signals::storage<void (float), signals::output::fused> floater(2.5f);
        signals::storage<void (float), signals::output::fused> collector(0.0f);

        // create the network (banger to floater.send, floater to collector)
        banger >>= floater.send_slot();
        floater >>= collector;
        
        // signal from banger causes floater to output 2.5
        banger();
        BOOST_CHECK_EQUAL(collector.at<0>(), 2.5f);
        
        // change the value in floater
        floater.close();
        floater(1.5f);
        invoke(floater); // we can also signal floater directly
        BOOST_CHECK_EQUAL(collector.at<0>(), 1.5f);
        //]
    }
    return 0;
} // int test_main(int, char* [])