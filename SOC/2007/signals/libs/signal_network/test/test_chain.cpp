// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/chain.hpp>
#include <boost/signal_network/storage.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/fusion/sequence/generation/make_vector.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

//[ test_chain_classes
class UnfusedDoublerClass : public signet::filter<void (float), signet::unfused_out_signal>
{
public:
    typedef void result_type;
	void operator()(float x) {out(2*x);}
};

class FusedDoublerClass : public signet::filter<void (float), signet::fused_out_signal>
{
public:
    typedef void result_type;
	void operator()(const fusion::vector<float> &x)
    {
        // this should be more general but having problems with the general approach...
        fusion::vector<float> y;
        boost::fusion::at_c<0>(y) = 2 * boost::fusion::at_c<0>(x);
        fused_out(y);
    }
};
//]

int test_main(int, char* [])
{
    {
        //[ test_chain_unfused
        signet::chain<UnfusedDoublerClass, void(float)>::unfused doubling_chain(4, NULL);
        signet::storage<void (float)>::unfused floater(1.0f);
        signet::storage<void (float)>::unfused collector(0.0f);

        floater >>= doubling_chain >>= collector;
        floater();

        BOOST_CHECK_EQUAL(collector.at<0>(), 16.0f);
        //]
    }
    {
        //[ test_chain_fused
        signet::chain<FusedDoublerClass, void(float)> doubling_chain(4, NULL);
        signet::storage<void (float)> floater(1.0f);
        signet::storage<void (float)> collector(0.0f);
        
        floater >>= doubling_chain >>= collector;
        floater();
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 16.0f);
        //]
    }
    return 0;
} // int test_main(int, char* [])
