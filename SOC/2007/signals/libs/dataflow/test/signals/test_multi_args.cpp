// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/component/storage.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/optional.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

//[ test_multi_args_class1

class SignalFloatDuplicator : public signals::filter<void (float, float), signals::unfused>
{
public:
    typedef void result_type;
	void operator()(float val) {out(val, val);}
};

//]

//[ test_multi_args_class2

class SignalMultiCollector
    : public boost::dataflow::port<boost::dataflow::signals::keyed_consumer>
{
	optional<float> last, last1, last2;
	int cnt;
public:
    typedef void result_type;
    
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

//]

int test_main(int, char* [])
{
    {
        //[ test_multi_args_unfused
        signals::storage<void (), signals::unfused> banger;
        signals::storage<void (float), signals::unfused> floater;
        floater(2.5f);
        SignalFloatDuplicator duplicator;
        SignalMultiCollector collector;
        
        banger
            | collector
            |
            (floater
             | collector
             | (duplicator >>= collector)).send_slot();
        
        banger();
        BOOST_CHECK_EQUAL(collector.GetCount(), 1);
        BOOST_CHECK_EQUAL(collector.GetLast(), optional<float>(2.5f));
        BOOST_CHECK_EQUAL(collector.GetLast1(), optional<float>(2.5f));
        BOOST_CHECK_EQUAL(collector.GetLast2(), optional<float>(2.5f));
        //]
    }
    return 0;
} // int test_main(int, char* [])
