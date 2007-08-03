// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/component/storage.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

//[ test_multi_out_classes

class SignalOutIntFloat : public signals::filter<void (float), signals::unfused>
{
public:
    SignalOutIntFloat(float x) : x(x) {}
    void operator()()
    {
        out(x);
        out_int((int)x);
    }
    boost::signal<void (int)> out_int;
private:
    float x;
}; // end class SignalOutIntFloat

//]

int test_main(int, char* [])
{
    {
        //[ test_multi_out_unfused

        SignalOutIntFloat multi_out(2.5f);
        signals::storage<void (float), signals::unfused> float_collector(0);
        signals::storage<void (int), signals::unfused> int_collector(0);

        multi_out >>= float_collector;
        multi_out.out_int >>= int_collector;
        multi_out();

        BOOST_CHECK_EQUAL(float_collector.at<0>(), 2.5f);
        BOOST_CHECK_EQUAL(int_collector.at<0>(), 2);
        
        //]
    }
return 0;
} // int test_main(int, char* [])


