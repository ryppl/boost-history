// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/counter.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

#include <boost/test/included/test_exec_monitor.hpp>

int test_main(int, char* [])
{
    signet::counter<void ()>::unfused counter1;
	
    for (int i=0; i<33; i++)
        counter1();
    BOOST_CHECK_EQUAL(counter1.count(), 33);

    return 0;
} // int test_main(int, char* [])