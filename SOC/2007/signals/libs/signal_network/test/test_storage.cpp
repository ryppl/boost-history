// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/storage.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

#include <boost/test/included/test_exec_monitor.hpp>

int test_main(int, char* [])
{
	// instantiate all of the components we need
    signet::storage<void ()>::unfused banger;
	signet::storage<void (float)>::unfused floater(2.5f);
	signet::storage<void (float)>::unfused collector(0.0f);

	// create the network
	banger >>= floater >>= collector;

	banger(); // signal from banger will now cause floater to output 2.5
	BOOST_CHECK_EQUAL(collector.value_<0>(), 2.5f);

	floater(1.5f); // change the value in floater
	floater(); // we can also signal floater directly
	BOOST_CHECK_EQUAL(collector.value_<0>(), 1.5f);
    return 0;
} // int test_main(int, char* [])