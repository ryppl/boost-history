// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/counter.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

int test_main(int, char* [])
{
    {
        //[ test_counter_unfused
        signals::counter<void ()> counter;
        
        for (int i=0; i<33; i++)
            counter();
        BOOST_CHECK_EQUAL(counter.count(), 33);
        //]
    }
    {
        //[ test_counter_fused
        signals::counter<void (), signals::output::fused> counter;
        
        for (int i=0; i<33; i++)
            counter(boost::fusion::vector<>());
        BOOST_CHECK_EQUAL(counter.count(), 33);
        //]
    }
    
    return 0;
} // int test_main(int, char* [])
