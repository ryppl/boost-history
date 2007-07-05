// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/component/chain.hpp>
#include <boost/signal_network/component/storage.hpp>
#include <boost/signal_network/connection.hpp>

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/sequence/generation/make_vector.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

//[ test_chain_classes
class UnfusedDoublerClass : public signals::filter<void (float), signals::unfused>
{
public:
    typedef void result_type;
	void operator()(float x) {out(2*x);}
};

class FusedDoublerClass : public signals::filter<void (float), signals::fused>
{
public:
    typedef void result_type;
	void operator()(const fusion::vector<float> &x)
    {
        // this could be more general but I'm having problems with the general approach...
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
        signals::chain<void(float), UnfusedDoublerClass> doubling_chain(4, NULL);
        signals::storage<void (float)> floater(1.0f);
        signals::storage<void (float)> collector(0.0f);

        floater >>= doubling_chain >>= collector;
        floater.send();

        BOOST_CHECK_EQUAL(collector.at<0>(), 16.0f);
        //]
    }
    {
        //[ test_chain_fused
        signals::chain<void(float), FusedDoublerClass> doubling_chain(4, NULL);
        signals::storage<void (float)> floater(1.0f);
        signals::storage<void (float)> collector(0.0f);
        
        floater >>= doubling_chain >>= collector;
        floater.send();
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 16.0f);
        //]
    }
    return 0;
} // int test_main(int, char* [])
