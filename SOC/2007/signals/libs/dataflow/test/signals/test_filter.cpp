// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/filter.hpp>
#include <boost/dataflow/signals/connection/operators.hpp>

#include <boost/fusion/sequence/intrinsic/at.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

//[ test_filter_classes

class DoublerClass : public signals::filter<DoublerClass, void (int)>
{
public:
    void operator()(int x) {out(2*x);}
};

struct receiver
    : public boost::dataflow::port<boost::dataflow::signals::call_consumer>    
{
    receiver() : stored(0) {}
    
    void operator()(int x)
    {
        stored = x;
    }
    void operator()(const fusion::vector<int> &x)
    {
        stored = fusion::at_c<0>(x);
    }
    
    int stored;
};

class FusedDoublerClass : public signals::filter<FusedDoublerClass, void (int), signals::fused>
{
public:
    void operator()(const fusion::vector<int> &x)
    {
        // this could be more general but I'm having problems with the general approach...
        fusion::vector<int> y;
        fusion::at_c<0>(y) = 2 * fusion::at_c<0>(x);
        fused_out(y);
    }
};

//]

int test_main(int, char* [])
{    
    namespace df=boost::dataflow;
    {
        //[ test_filter_unfused

        DoublerClass doubler1, doubler2;
        receiver collector;
        
        doubler1 >>= doubler2 >>= collector;
        doubler1(1);
        
        BOOST_CHECK_EQUAL(collector.stored, 4);
        
        //]
    }
    {
        //[ test_filter_fused
        
        FusedDoublerClass doubler1, doubler2;
        receiver collector;
        
        doubler1 >>= doubler2 >>= collector;
        doubler1(1);
        
        BOOST_CHECK_EQUAL(collector.stored, 4.0f);
        
        //]
    }
    return 0;
} // int test_main(int, char* [])
