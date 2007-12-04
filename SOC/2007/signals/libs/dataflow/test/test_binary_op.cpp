// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "my_producer_consumer.hpp"
#include "my_producer_consumer_operations.hpp"

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;

struct whatever;

int test_main(int, char* [])
{
    my_producer p;
    my_consumer c;
    
    df::binary_operation<df::operations::connect, df::default_tag>(p, c);
    BOOST_CHECK(connected);
    
    df::binary_operation<df::operations::connect_only, df::default_tag>(p, c);
    BOOST_CHECK(only_connected);

    df::binary_operation<df::operations::disconnect, df::default_tag>(p, c);
    BOOST_CHECK(disconnected);
    
    connected = false;
    df::binary_operation<df::operations::connect, whatever, df::default_tag>(p, c);
    BOOST_CHECK(connected);
    
    return 0;
} // int test_main(int, char* [])

