// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/function.hpp>
#include <boost/signal_network/storage.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

#include <boost/test/included/test_exec_monitor.hpp>

int test_main(int, char* [])
{
    {
        //[ test_function_unfused
        signet::function<void (float), float(float)>::unfused
            double_fun1(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signet::function<void (float), float(float)>::unfused
            double_fun2(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signet::storage<void (float)>::unfused floater(1.0f);
        signet::storage<void (float)>::unfused collector(0.0f);

        floater >>= double_fun1 >>= double_fun2 >>= collector;
        floater();

        BOOST_CHECK_EQUAL(collector.at<0>(), 4.0f);
        //]
    }
    {
        //[ test_function_fused
        signet::function<void (float), float(float)>
            double_fun1(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signet::function<void (float), float(float)>
            double_fun2(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signet::storage<void (float)> floater(1.0f);
        signet::storage<void (float)> collector(0.0f);
        
        floater >>= double_fun1 >>= double_fun2 >>= collector;
        floater();
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 4.0f);
        //]
    }    
    return 0;
} // int test_main(int, char* [])
