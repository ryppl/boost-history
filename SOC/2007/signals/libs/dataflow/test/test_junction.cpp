// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/component/junction.hpp>
#include <boost/dataflow/signal/component/counter.hpp>
#include <boost/dataflow/signal/component/storage.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

int test_main(int, char* [])
{
    {
        //[ test_junction_unfused
        signals::storage<void (), signals::unfused> banger1, banger2;
        signals::counter<void (), signals::unfused> counter1, counter2;
        signals::junction<void (), signals::unfused> junction;
        
        banger1 >>= junction >>= counter1;
        banger2 >>= junction >>= counter2;

        banger1.send();
        banger2.send();
        
        BOOST_CHECK_EQUAL(counter1.count(), 2);
        BOOST_CHECK_EQUAL(counter2.count(), 2);

        junction.disable();
        banger1.send();
        banger2.send();

        BOOST_CHECK_EQUAL(counter1.count(), 2);
        BOOST_CHECK_EQUAL(counter2.count(), 2);

        junction.enable();
        banger1.send();
        banger2.send();

        BOOST_CHECK_EQUAL(counter1.count(), 4);
        BOOST_CHECK_EQUAL(counter2.count(), 4);
        //]
    }
    {
        //[ test_junction_fused
        signals::storage<void (), signals::fused> banger1, banger2;
        signals::counter<void (), signals::fused> counter1, counter2;
        signals::junction<void (), signals::fused> junction;
        
        banger1 >>= junction >>= counter1;
        banger2 >>= junction >>= counter2;
        
        banger1();
        banger2();
        
        BOOST_CHECK_EQUAL(counter1.count(), 2);
        BOOST_CHECK_EQUAL(counter2.count(), 2);
        
        junction.disable();
        banger1();
        banger2();
        
        BOOST_CHECK_EQUAL(counter1.count(), 2);
        BOOST_CHECK_EQUAL(counter2.count(), 2);
        
        junction.enable();
        banger1();
        banger2();
        
        BOOST_CHECK_EQUAL(counter1.count(),  4);
        BOOST_CHECK_EQUAL(counter2.count(), 4);
        //]
    }    
    return 0;
} // int test_main(int, char* [])