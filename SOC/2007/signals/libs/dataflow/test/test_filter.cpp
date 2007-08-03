// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/component/filter.hpp>
#include <boost/dataflow/signal/component/storage.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/fusion/sequence/intrinsic/at.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

//[ test_filter_classes

class DoublerClass : public signals::filter<void (float), signals::unfused>
{
public:
    template<typename FArgs>
    struct result;

    template<typename F>
    struct result<F(float)>
    {
        typedef void type;
    };
    
    void operator()(float x) {out(2*x);}
};

class FusedDoublerClass : public signals::filter<void (float), signals::fused>
{
public:
    template<typename FArgs>
    struct result;

    template<typename F>
    struct result<F(const fusion::vector<float> &)>
    {
        typedef void type;
    };

    void operator()(const fusion::vector<float> &x)
    {
        // this could be more general but I'm having problems with the general approach...
        fusion::vector<float> y;
        fusion::at_c<0>(y) = 2 * fusion::at_c<0>(x);
        fused_out(y);
    }
};

//]

int test_main(int, char* [])
{    
    {
        //[ test_filter_unfused

        DoublerClass doubler1, doubler2;
        signals::storage<void (float)> floater(1.0f);
        signals::storage<void (float)> collector(0.0f);

        floater >>= doubler1 >>= doubler2 >>= collector;
        floater.send();
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 4.0f);
        
        //]
    }
    {
        //[ test_filter_fused
        
        FusedDoublerClass doubler1, doubler2;
        signals::storage<void (float), signals::fused> floater(1.0f);
        signals::storage<void (float), signals::fused> collector(0.0f);
        
        floater >>= doubler1 >>= doubler2 >>= collector;
        floater.send();
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 4.0f);
        
        //]
    }
    return 0;
} // int test_main(int, char* [])
