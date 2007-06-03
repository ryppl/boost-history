// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/junction.hpp>
#include <boost/signal_network/counter.hpp>
#include <boost/signal_network/storage.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

#include <boost/test/included/test_exec_monitor.hpp>

int test_main(int, char* [])
{
	signet::storage<void ()>::unfused banger1, banger2;
    signet::counter<void ()>::unfused counter1, counter2;
    signet::junction<void ()>::unfused junction;
	
	banger1 >>= junction >>= counter1;
	banger2 >>= junction >>= counter2;

	banger1();
	banger2();

	BOOST_CHECK(counter1.count() == 2);
	BOOST_CHECK(counter2.count() == 2);

	junction.disable();
	banger1();
	banger2();

	BOOST_CHECK(counter1.count() == 2);
	BOOST_CHECK(counter2.count() == 2);

	junction.enable();
	banger1();
	banger2();

	BOOST_CHECK(counter1.count() == 4);
	BOOST_CHECK(counter2.count() == 4);

    return 0;
} // int test_main(int, char* [])