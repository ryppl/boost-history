// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/function.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

int test_main(int, char* [])
{
    {
        //[ test_function_unfused
        signals::function<void (float), float(float)>
            double_fun1(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signals::function<void (float), float(float)>
            double_fun2(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signals::storage<void (float)> floater(1.0f);
        signals::storage<void (float)> collector(0.0f);

        floater >>= double_fun1 >>= double_fun2 >>= collector;
        floater.send();

        BOOST_CHECK_EQUAL(collector.at<0>(), 4.0f);
        //]
    }
    {
        //[ test_function_fused
        signals::function<void (float), float(float), signals::output::fused>
            double_fun1(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signals::function<void (float), float(float), signals::output::fused>
            double_fun2(boost::bind(std::multiplies<float>(), _1, 2.0f));
        signals::storage<void (float), signals::output::fused> floater(1.0f);
        signals::storage<void (float), signals::output::fused> collector(0.0f);
        
        floater >>= double_fun1 >>= double_fun2 >>= collector;
        floater.send();
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 4.0f);
        //]
    }    
    return 0;
} // int test_main(int, char* [])
