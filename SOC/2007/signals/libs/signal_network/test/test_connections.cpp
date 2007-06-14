// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define _CRT_SECURE_NO_DEPRECATE 1
#include <boost/fusion/sequence/generation/make_vector.hpp>


#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

#define BOOST_SIGNALS_STATIC_LINK
#include <boost/optional.hpp>
#include <boost/signal_network/filter.hpp>
#include <boost/signal_network/storage.hpp>
#include <boost/signal_network/junction.hpp>
#include <boost/signal_network/selector.hpp>
#include <boost/signal_network/mutex.hpp>
#include <boost/signal_network/timed_generator.hpp>
#include <boost/signal_network/function.hpp>
#include <boost/signal_network/chain.hpp>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define _WIN32_WINDOWS
#endif
#include <boost/signal_network/socket_receiver.hpp>
#include <boost/signal_network/socket_sender.hpp>
#undef _WIN32_WINDOWS

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

class SignalFloatDoubler : public signet::filter<void (float, float)>
{
public:
	void operator()(float val) {out(val, val*2);}
};


class SignalFloat2Collector
{
	optional<float> last1, last2;
public:
	void operator()(float val1, float val2)
	{
		last1 = val1;
		last2 = val2;
	}
	optional<float> GetLast1()
	{
		return last1;
	}
	optional<float> GetLast2()
	{
		return last2;
	}
};

class SignalMultiInheritedCollector : public signet::storage<void (float)>::unfused, public SignalVoidCounter, public SignalFloat2Collector
{
public:
    SignalMultiInheritedCollector() : signet::storage<void (float)>::unfused(0) {}
};

void multi_num_args_inherited_test()
{
	signet::storage<void ()>::unfused banger;
	signet::storage<void (float)>::unfused floater;
	floater(2.5f);
	SignalFloatDuplicator duplicator;
	SignalMultiInheritedCollector collector;
	
	banger
		| (SignalVoidCounter &) collector
		|
		(floater
			| (signet::storage<void (float)>::unfused &) collector
			| (duplicator >>= (SignalFloat2Collector &) collector));

	banger();
	BOOST_CHECK(collector.GetCount() == 1);
	BOOST_CHECK(collector.value_<0>() == 2.5f); // calls the collector<float>'s operator()
	BOOST_CHECK(collector.GetLast1() == optional<float>(2.5f));
	BOOST_CHECK(collector.GetLast2() == optional<float>(2.5f));
} // end void multi_num_args_inherited_test()




void selector_test()
{
/*	signet::storage<void ()>::unfused banger;
	signet::storage<void (float)>::unfused floater1, floater2;
	floater1(1.0f);
	floater2(2.0f);
	signet::storage<void (float)>::unfused collector(0.0f);
	signet::selector<2, void (float)> selector;

	banger >>= floater1 >>= selector.slot1();
	banger >>= floater2 >>= selector.slot2();
	selector >>= collector;

	selector.select(0);
	banger();
	BOOST_CHECK(collector.value_<0>() == 0.0f);

	selector.select(1);
	banger();
	BOOST_CHECK(collector.value_<0>() == 1.0f);

	selector.select(2);
	banger();
	BOOST_CHECK(collector.value_<0>() == 2.0f);*/
} // end void selector_test()





