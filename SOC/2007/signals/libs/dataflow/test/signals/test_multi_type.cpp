// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/component/storage.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/optional.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

//[ test_multi_type_classes

class SignalIntFloatCollector : public boost::dataflow::port<
    boost::dataflow::signals::call_consumer>
{
    optional<int> last_int;
    optional<float> last_float;
public:
    typedef void result_type;
    
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

//]

int test_main(int, char* [])
{
    {
        //[ test_multi_type_unfused
        signals::storage<void (), signals::unfused> banger;
        signals::storage<void (int), signals::unfused> inter;
        inter(2);
        signals::storage<void (float), signals::unfused> floater;
        floater(3.3f);
        SignalIntFloatCollector collector;
        
        banger
            | (inter >>= collector).send_slot()
            | (floater >>= collector).send_slot();
        
        banger();
        BOOST_CHECK_EQUAL(collector.GetLastInt(), optional<int>(2));
        BOOST_CHECK_EQUAL(collector.GetLastFloat(), optional<float>(3.3f));
        //]
    }
    return 0;
} // int test_main(int, char* [])
