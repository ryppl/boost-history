// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/optional.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

//[ test_multi_type_classes

class SignalIntFloatCollector : public signals::consumer<SignalIntFloatCollector>
{
public:    
    void operator()(int x)
    {
        last_int = x;
    }
    void operator()(float x)
    {
        last_float = x;
    }

    optional<int> last_int;
    optional<float> last_float;
}; // end class SignalIntFloatCollector

//]

int test_main(int, char* [])
{
    {
        //[ test_multi_type_unfused
        signals::storage<void ()> banger;
        signals::storage<void (int)> int_storage(2);
        signals::storage<void (float)> float_storage(3.3f);
        SignalIntFloatCollector collector;
        
        // ---Connect the dataflow network -----------------------------
        //
        //
        //            ,---------------.  void(float)   
        //            |  int_storage  | -------------+ 
        //            `--(send_slot)--'              |  
        //                    ^                      |
        // ,--------.  void() |                      +>,-----------.
        // | banger | --------+                        | collector |
        // `--------'         |                      +>`-----------'
        //                    V                      |
        //            ,--(send_slot)--.  void(float) | 
        //            | float_storage | -------------+ 
        //            `---------------'                
        //
        // -------------------------------------------------------------

        banger
            | int_storage.send_slot()
            | float_storage.send_slot();
        int_storage >>= collector;
        float_storage >>= collector;
        
        banger(); // this causes int_storage to output 2, and float_storage 3.3
        BOOST_CHECK_EQUAL(collector.last_int, optional<int>(2));
        BOOST_CHECK_EQUAL(collector.last_float, optional<float>(3.3f));
        //]
    }
    return 0;
} // int test_main(int, char* [])
