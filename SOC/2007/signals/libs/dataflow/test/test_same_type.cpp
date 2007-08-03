// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/component/storage.hpp>
#include <boost/dataflow/signal/component/counter.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

//[ test_same_type_classes

class Signal2VoidCounter : public signals::counter<void (), signals::unfused>
{
public:
	signals::counter<void (), signals::unfused> other;
}; // end class Signal2VoidCounter

class Signal2VoidInputs
{
	int result;
public:
    typedef void result_type;
    typedef dataflow::signal_consumer consumer_category;
    
	Signal2VoidInputs() : result(0) {};
	void operator()()
	{
		result++;
	}
	void AltInput()
	{
		result+=10;
	}
	int GetResult()
	{
		return result;
	}
}; // end class Signal2VoidInputs

//]

int test_main(int, char* [])
{
    {
        //[ test_same_type_unfused
        signals::storage<void (), signals::unfused> banger;
        Signal2VoidCounter counter;
        
        banger
            | counter
            | counter.other;
        
        banger();
        BOOST_CHECK_EQUAL(counter.count(), 1);
        BOOST_CHECK_EQUAL(counter.other.count(), 1);
        
        Signal2VoidInputs inputs;
        
        banger
            | inputs
            | signals::make_slot_selector<void ()> (&Signal2VoidInputs::AltInput, inputs);
        
        banger();
        BOOST_CHECK_EQUAL(inputs.GetResult(), 11);
        //]
    }
    return 0;
} // int test_main(int, char* [])
