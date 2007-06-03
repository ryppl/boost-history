// SignalLinkTests.cpp

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

class SignalVoidCounter
#ifdef SIGNAL_NETWORK_TRACKABLE
	// Two base signals implementations are supported currently
	// (Boost.Signals by Doug Gregor and the
	// thread_safe_signals version under implementation by Frank Mori Hess).
	// SIGNAL_NETWORK_TRACKABLE is defined if you are using Boost.Signals
	// - in this case, we want to make our component trackable so that
	// it gets disconnected automatically when this object is destroyed.
	: public boost::signals::trackable
#endif
{
	volatile int cnt;
public:
	SignalVoidCounter() : cnt(0) {}
	void operator()()
	{
		cnt++; // whenever a void() signal is received, increase the counter
	}
	int GetCount()
	{
		return cnt;
	}
}; // end class SignalVoidCounter

class SignalIntFloatCollector
{
	optional<int> last_int;
	optional<float> last_float;
public:
	void operator()(int x)
	{
		last_int = x;
	}
	void operator()(float x)
	{
		last_float = x;
	}
	optional<int> GetLastInt()
	{
		return last_int;
	}
	optional<float> GetLastFloat()
	{
		return last_float;
	}
}; // end class SignalIntFloatCollector

class SignalFloatDoubler : public signet::filter<void (float, float)>
{
public:
	void operator()(float val) {out(val, val*2);}
};

class SignalFloatDuplicator : public signet::filter<void (float, float)>
{
public:
	void operator()(float val) {out(val, val);}
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

void simple_test()
{
	// instantiate all of the components we need
    signet::storage<void ()>::unfused banger;
	signet::storage<void (float)>::unfused floater(2.5f);
	signet::storage<void (float)>::unfused collector(0.0f);

	// create the network
	banger >>= floater >>= collector;

	banger(); // signal from banger will now cause floater to output 2.5
	BOOST_CHECK(collector.value_<0>() == 2.5f);

	floater(1.5f); // change the value in floater
	floater(); // we can also signal floater directly
	BOOST_CHECK(collector.value_<0>() == 1.5f);
} // end void simple_test()

void branching_test()
{
    signet::storage<void ()>::unfused banger;
	SignalVoidCounter counter;
    signet::storage<void (float)>::unfused floater;
	floater(2.5f);

    signet::storage<void (float)>::unfused collector(0.0f);
	
	banger
		| (floater >>= collector) // floater connects to collector, banger to floater
		| counter; // and banger to counter
		
	banger();
	BOOST_CHECK(counter.GetCount() == 1);
	BOOST_CHECK(collector.value_<0>() == 2.5f);
} // end void branching_test()

void disconnect_test()
{
    signet::storage<void ()>::unfused banger;
	{
		SignalVoidCounter counter;
        signet::storage<void (float)>::unfused floater;
		floater(2.5f);
        signet::storage<void (float)>::unfused collector(0.0f);

		banger
			| counter
			| (floater >>= collector);

		banger();
		BOOST_CHECK(counter.GetCount() == 1);
		BOOST_CHECK(collector.value_<0>() == 2.5f);
	} // counter, floater, and collector are now destroyed and disconnected with Boost.Signals
#ifdef SIGNAL_NETWORK_THREAD_SAFE
	// if Signal Network has detected thread safe signals, we need to
	// touch the signal to disconnect things that are destroyed:
	// unfortunatelly, this call hangs on MSVC!
	// banger();
#endif
	BOOST_CHECK(banger.default_signal().num_slots() == 0); 

	SignalVoidCounter counter;

	banger >>= counter;
	banger.disconnect_all_slots();

	banger();
	BOOST_CHECK(counter.GetCount() == 0);
} // end void disconnect_test

void multi_type_test()
{
    signet::storage<void ()>::unfused banger;
	signet::storage<void (int)>::unfused inter;
	inter(2);
	signet::storage<void (float)>::unfused floater;
	floater(3.3f);
	SignalIntFloatCollector collector;

	banger
		| (inter >>= collector)
		| (floater >>= collector);

	banger();
	BOOST_CHECK(collector.GetLastInt() == optional<int>(2));
	BOOST_CHECK(collector.GetLastFloat() == optional<float>(3.3f));
} // end void multi_type_test()

class SignalMultiCollector
{
	optional<float> last, last1, last2;
	int cnt;
public:
	SignalMultiCollector() : cnt(0) {}
	void operator()()
	{
		cnt++;
	}
	int GetCount()
	{
		return cnt;
	}
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
	void operator()(float x)
	{
		last = x;
	}
	optional<float> GetLast()
	{
		return last;
	}
}; // end class SignalMultiCollector

void multi_num_args_test()
{
	signet::storage<void ()>::unfused banger;
	signet::storage<void (float)>::unfused floater;
	floater(2.5f);
	SignalFloatDuplicator duplicator;
	SignalMultiCollector collector;
	
	banger
		| collector
		|
		(floater
			| collector
			| (duplicator >>= collector));

	banger();
	BOOST_CHECK(collector.GetCount() == 1);
	BOOST_CHECK(collector.GetLast() == optional<float>(2.5f));
	BOOST_CHECK(collector.GetLast1() == optional<float>(2.5f));
	BOOST_CHECK(collector.GetLast2() == optional<float>(2.5f));
} // end void multi_num_args_test()


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

class SignalOutIntFloat : public signet::filter<void (float)>
{
public:
	SignalOutIntFloat(float x) : x(x) {}
	void operator()()
	{
		out(x);
		out_int((int)x);
	}
	boost::signal<void (int)> out_int;
private:
	float x;
}; // end class SignalOutIntFloat

void multi_out_test()
{
	SignalOutIntFloat multi_out(2.5f);
	SignalIntFloatCollector collector;
	
	multi_out >>= collector;
	multi_out.out_int >>= collector;
	multi_out();
	
	BOOST_CHECK(collector.GetLastFloat() == optional<float>(2.5f));
	BOOST_CHECK(collector.GetLastInt() == optional<int>(2));
} // end void multi_out_test()

class Signal2VoidCounter : public SignalVoidCounter
{
public:
	SignalVoidCounter other;
}; // end class Signal2VoidCounter

class Signal2VoidInputs : public signet::filter<void(int)>
{
	int result;
public:
	Signal2VoidInputs() : result(0) {};
	void operator()()
	{
		result++;
		out(result);
	}
	void AltInput()
	{
		result+=10;
		out(result);
	}
	int GetResult()
	{
		return result;
	}
}; // end class Signal2VoidInputs

void multi_in_test()
{
	signet::storage<void ()>::unfused banger;
	Signal2VoidCounter counter;
	
	banger
		| counter
		| counter.other;
	
	banger();
	BOOST_CHECK(counter.GetCount() == 1);
	BOOST_CHECK(counter.other.GetCount() == 1);

	Signal2VoidInputs inputs;

	banger
		| inputs
		| slot_selector<void ()> (inputs, &Signal2VoidInputs::AltInput);

	banger();
	BOOST_CHECK(inputs.GetResult() == 11);
}; // end void multi_in_test

void junction_test()
{
	signet::storage<void ()>::unfused banger1, banger2;
	SignalVoidCounter counter1, counter2;
    signet::junction<void ()>::unfused junction;
	
	banger1 >>= junction >>= counter1;
	banger2 >>= junction >>= counter2;

	banger1();
	banger2();

	BOOST_CHECK(counter1.GetCount() == 2);
	BOOST_CHECK(counter2.GetCount() == 2);

	junction.disable();
	banger1();
	banger2();

	BOOST_CHECK(counter1.GetCount() == 2);
	BOOST_CHECK(counter2.GetCount() == 2);

	junction.enable();
	banger1();
	banger2();

	BOOST_CHECK(counter1.GetCount() == 4);
	BOOST_CHECK(counter2.GetCount() == 4);
} // end void junction_test()

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

#include <iostream>
using namespace std;

struct ticker
{
	void operator()()
	{
		cout << "tick" << endl;
	}
};

void mutex_test()
{
	signet::timed_generator<void ()> banger1;
	signet::timed_generator<void ()> banger2;
    signet::mutex<void ()>::unfused lock;
	SignalVoidCounter counter;
	ticker tick;

	banger1 >>= lock >>= counter;
	banger2 >>= lock >>= tick;
	banger1.enable(0.5, 5);
	banger2.enable(0.5, 5);

	while (counter.GetCount() < 10) {}

	BOOST_CHECK(counter.GetCount() == 10);
	banger1.join();
	banger2.join();
} // end void mutex_test

class DoublerClass : public signet::filter<void (float)>
{
public:
	void operator()(float x) {out(2*x);}
};

void filter_test()
{
	DoublerClass doubler1, doubler2;
	signet::storage<void (float)>::unfused floater(1.0f);
	signet::storage<void (float)>::unfused collector(0.0f);

	floater >>= doubler1 >>= doubler2 >>= collector;
	floater();

	BOOST_CHECK(collector.value_<0>() == 4.0f);
} // end void filter_test()

float DoublerFunc(float  x)
{
	return x*2;
}

void function_test()
{
    signet::function<float(float)>::unfused double_fun1(&DoublerFunc);
    signet::function<float(float)>::unfused double_fun2(&DoublerFunc);
	signet::storage<void (float)>::unfused floater(1.0f);
	signet::storage<void (float)>::unfused collector(0.0f);

	floater >>= double_fun1 >>= double_fun2 >>= collector;
	floater();

	BOOST_CHECK(collector.value_<0>() == 4.0f);
} // end void function_test()

void chain_test()
{
    signet::chain<DoublerClass, void(float)>::unfused doubling_chain(4, NULL);
	signet::storage<void (float)>::unfused floater(1.0f);
	signet::storage<void (float)>::unfused collector(0.0f);

	floater >>= doubling_chain >>= collector;
	floater();

	BOOST_CHECK(collector.value_<0>() == 16.0f);
} // end void chain_test

class PullDoubler : public signet::filter<float ()>
{
public:
	float operator()()
	{
		return 2*out();
	}
};

void pull_test()
{
	signet::storage<void(float)>::unfused generator(1.0f);
	PullDoubler doubler;

	doubler >>= generator.slot<0>();

	BOOST_CHECK(doubler() == 2.0f);
} // end void pull_test

// asio test
boost::mutex mutex_;
boost::condition cond;
asio::io_service io_service;

// This function will set up an asio acceptor, and wait for a connection.
// Once established, it will set up a signal network which will send
// its final signal through the socket.
void asio_server()
{
	// set up the socket
	asio::ip::tcp::acceptor acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1097));
	asio::ip::tcp::socket socket(io_service);
	{
		boost::mutex::scoped_lock lock(mutex_);
		acceptor.listen();
		cond.notify_all();
	}
	acceptor.accept(socket);

	// instantiate the components - a float generator, a filter that adds 2, and a sender
	signet::storage<void (float)>::unfused generator(1.0f);
    signet::function<float(float)>::unfused add2(boost::bind(std::plus<float>(), _1, 2.0f));
    signet::socket_sender<void (float)>::unfused sender(socket);

	// create the network
	generator >>= add2 >>= sender;

	// cause the generator to send it's stored value
	generator();
}

// main test function
void asio_test()
{
	// start the server in a separate thread
	boost::mutex::scoped_lock lock(mutex_);
	boost::thread t(asio_server);
	cond.wait(lock);

	// set up the socket
	asio::ip::tcp::endpoint endpoint_recv(asio::ip::address::from_string("127.0.0.1"), 1097);
	asio::ip::tcp::socket socket(io_service);
	socket.connect(endpoint_recv);

	// instatiate the components
    signet::socket_receiver<void (float)> receiver(socket);
	signet::storage<void (float)>::unfused collector(0.0f);

	// set up the network
	receiver >>= collector;

	// this receiver is synchronous - we have to tell it to receive a signal
	receiver();

	BOOST_CHECK(collector.value_<0>() == 3.0f);

	t.join();
} // end void asio_test

test_suite* init_unit_test_suite(int argc, char* argv[])
{
    signet::storage<void (int, int)> s;
    boost::fusion::vector<int, int> val(4, 5);
    s(val);
    std::cout << s.value_<0>() << s.value_<1>() << std::endl;
    signet::storage<void (int, int) >::unfused s2;

    s >>= s2;
    s();
    std::cout << s2.value_<0>() << s2.value_<1>() << std::endl;

    signet::storage<void (int, int) >::unfused s3(5, 6);
    std::cout << s3.value_<0>() << s3.value_<1>() << std::endl;
    s3(7, 8);
    std::cout << s3.value_<0>() << s3.value_<1>() << std::endl;


    test_suite* test = BOOST_TEST_SUITE( "Utility test suite" );

	test->add(BOOST_TEST_CASE(&simple_test));
	test->add(BOOST_TEST_CASE(&branching_test));
	test->add(BOOST_TEST_CASE(&disconnect_test));
	test->add(BOOST_TEST_CASE(&multi_type_test));
	test->add(BOOST_TEST_CASE(&multi_num_args_test));
	test->add(BOOST_TEST_CASE(&multi_num_args_inherited_test));
	test->add(BOOST_TEST_CASE(&multi_out_test));
	test->add(BOOST_TEST_CASE(&multi_in_test));
	test->add(BOOST_TEST_CASE(&junction_test));
	test->add(BOOST_TEST_CASE(&selector_test));
	test->add(BOOST_TEST_CASE(&mutex_test));
	test->add(BOOST_TEST_CASE(&filter_test));
	test->add(BOOST_TEST_CASE(&function_test));
	test->add(BOOST_TEST_CASE(&chain_test));
	test->add(BOOST_TEST_CASE(&pull_test));
	test->add(BOOST_TEST_CASE(&asio_test));

	return test;
}
