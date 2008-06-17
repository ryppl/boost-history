// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/multiplexer.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

int test_main(int, char* [])
{
    {
        //[ test_multiplexer_unfused
        signals::storage<void (float)> one(1.0f), two(2.0f), collector(-1.0f);
        signals::multiplexer<void (float)> multiplexer;
        
        one >>= multiplexer.slot<1>();
        two >>= multiplexer.slot<2>();
        
        multiplexer >>= collector;
        
        invoke(one);
        invoke(two);
        
        BOOST_CHECK_EQUAL(collector.at<0>(), -1.0f);
        
        multiplexer.select(1);
        invoke(one);
        invoke(two);
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 1.0f);

        multiplexer.select(2);
        invoke(two);
        invoke(one);
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 2.0f);
        //]
    }
    {
        //[ test_multiplexer_fused
        signals::storage<void (float), signals::output::fused> one(1.0f), two(2.0f), collector(-1.0f);
        signals::multiplexer<void (float), signals::output::fused> multiplexer;
        
        one >>= multiplexer.slot<1>();
        two >>= multiplexer.slot<2>();
        
        multiplexer >>= collector;
        
        invoke(one);
        invoke(two);
        
        BOOST_CHECK_EQUAL(collector.at<0>(), -1.0f);
        
        multiplexer.select(1);
        invoke(one);
        invoke(two);
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 1.0f);

        multiplexer.select(2);
        invoke(two);
        invoke(one);
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 2.0f);
        //]
    }    
    return 0;
} // int test_main(int, char* [])