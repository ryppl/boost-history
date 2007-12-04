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
        //[ test_disconnect_unfused
        signals::storage<void ()> banger;        
        signals::counter<void ()> counter;
        
        banger >>= counter;
        banger.disconnect_all_slots();
        
        banger();
        BOOST_CHECK_EQUAL(counter.count(), 0);
        //]
    }
    return 0;
} // int test_main(int, char* [])


