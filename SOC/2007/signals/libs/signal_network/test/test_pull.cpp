// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/storage.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

//[ test_pull_classes
class PullDoubler : public signet::filter<float (), signet::unfused_out_signal>
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
        
        signet::storage<void(float)>::unfused generator(1.0f);
        PullDoubler doubler;
            
        doubler >>= generator.value_at_slot<0>();
            
        BOOST_CHECK(doubler() == 2.0f);
        //]
    }
    return 0;
} // int test_main(int, char* [])


