// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/component/function.hpp>
#include <boost/dataflow/signal/component/storage.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;
using namespace boost::dataflow::operators;

int test_main(int, char* [])
{
    {
        //[ test_function_unfused
        signals::function<void (float), float(float), signals::unfused>
            double_fun1(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signals::function<void (float), float(float), signals::unfused>
            double_fun2(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signals::storage<void (float), signals::unfused> floater(1.0f);
        signals::storage<void (float), signals::unfused> collector(0.0f);

        floater >>= double_fun1 >>= double_fun2 >>= collector;
        floater.send();

        BOOST_CHECK_EQUAL(collector.at<0>(), 4.0f);
        //]
    }
    {
        //[ test_function_fused
        signals::function<void (float), float(float), signals::fused>
            double_fun1(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signals::function<void (float), float(float), signals::fused>
            double_fun2(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signals::storage<void (float), signals::fused> floater(1.0f);
        signals::storage<void (float), signals::fused> collector(0.0f);
        
        floater >>= double_fun1 >>= double_fun2 >>= collector;
        floater.send();
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 4.0f);
        //]
    }    
    return 0;
} // int test_main(int, char* [])
