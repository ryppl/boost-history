// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/storage.hpp>
#include <boost/optional.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

//[ test_multi_type_classes

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

//]

int test_main(int, char* [])
{
    {
        //[ test_multi_type_unfused
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
        BOOST_CHECK_EQUAL(collector.GetLastInt(), optional<int>(2));
        BOOST_CHECK_EQUAL(collector.GetLastFloat(), optional<float>(3.3f));
        //]
    }
    return 0;
} // int test_main(int, char* [])
