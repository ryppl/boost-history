// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/junction.hpp>
#include <boost/dataflow/signals/component/counter.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

int test_main(int, char* [])
{
    {
        //[ test_junction_unfused
        signals::storage<void ()> banger1, banger2;
        signals::counter<void ()> counter1, counter2;
        signals::junction<void ()> junction;
        
        banger1 >>= junction >>= counter1;
        banger2 >>= junction >>= counter2;

        banger1.send();
        banger2.send();
        
        BOOST_CHECK_EQUAL(counter1.count(), 2);
        BOOST_CHECK_EQUAL(counter2.count(), 2);

        junction.close();
        banger1.send();
        banger2.send();

        BOOST_CHECK_EQUAL(counter1.count(), 2);
        BOOST_CHECK_EQUAL(counter2.count(), 2);

        junction.open();
        banger1.send();
        banger2.send();

        BOOST_CHECK_EQUAL(counter1.count(), 4);
        BOOST_CHECK_EQUAL(counter2.count(), 4);
        //]
    }
    {
        //[ test_junction_fused
        signals::storage<void (), signals::output::fused> banger1, banger2;
        signals::counter<void (), signals::output::fused> counter1, counter2;
        signals::junction<void (), signals::output::fused> junction;
        
        banger1 >>= junction >>= counter1;
        banger2 >>= junction >>= counter2;
        
        banger1();
        banger2();
        
        BOOST_CHECK_EQUAL(counter1.count(), 2);
        BOOST_CHECK_EQUAL(counter2.count(), 2);
        
        junction.close();
        banger1();
        banger2();
        
        BOOST_CHECK_EQUAL(counter1.count(), 2);
        BOOST_CHECK_EQUAL(counter2.count(), 2);
        
        junction.open();
        banger1();
        banger2();
        
        BOOST_CHECK_EQUAL(counter1.count(),  4);
        BOOST_CHECK_EQUAL(counter2.count(), 4);
        //]
    }    
    return 0;
} // int test_main(int, char* [])