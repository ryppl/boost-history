// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

//[ test_pull_classes
class PullDoubler : public signals::filter<PullDoubler, float ()>
{
public:
    float operator()()
    {
        return 2*out();
    }
};
//]

int test_main(int, char* [])
{
    {
        //[ test_pull_unfused
        
        signals::storage<void(float)> generator(1.0f);
        PullDoubler doubler;
        
        // ---Connect the dataflow network ---------------------
        //
        //     ,---------.    
        //     | doubler | <---------------. 
        //     `---------'                 |
        //                      ,-(value_at_slot<0>)-.
        //                      |      generator     |
        //                      `--------------------'
        //
        // -----------------------------------------------------
    
        doubler >>= generator.value_at_slot<0>();
            
        BOOST_CHECK(doubler() == 2.0f);
        //]
    }
    return 0;
} // int test_main(int, char* [])


