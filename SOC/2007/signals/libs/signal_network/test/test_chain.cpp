// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/chain.hpp>
#include <boost/signal_network/storage.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

#include <boost/test/included/test_exec_monitor.hpp>

class DoublerClass : public signet::filter<void (float)>
{
public:
	void operator()(float x) {out(2*x);}
};

int test_main(int, char* [])
{
    signet::chain<DoublerClass, void(float)>::unfused doubling_chain(4, NULL);
	signet::storage<void (float)>::unfused floater(1.0f);
	signet::storage<void (float)>::unfused collector(0.0f);

	floater >>= doubling_chain >>= collector;
	floater();

	BOOST_CHECK(collector.value_<0>() == 16.0f);

    return 0;
} // int test_main(int, char* [])
