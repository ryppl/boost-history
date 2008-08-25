// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/component/counter.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

//[ test_same_type_classes

class Signal2VoidInputs
    : public signals::consumer<Signal2VoidInputs>
{
public:
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
private:
    int result;
}; // end class Signal2VoidInputs

//]

int test_main(int, char* [])
{
    {
        //[ test_same_type_unfused
        signals::storage<void()> banger;
        Signal2VoidInputs inputs;
        
        banger
            | inputs // this will connect to operator()()
            | signals::bind_mem_fn(&Signal2VoidInputs::AltInput, inputs);
        
        banger();
        BOOST_CHECK_EQUAL(inputs.GetResult(), 11);
        //]
    }
    return 0;
} // int test_main(int, char* [])
