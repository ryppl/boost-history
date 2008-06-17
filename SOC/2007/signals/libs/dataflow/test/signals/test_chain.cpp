// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/chain.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/include/make_vector.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

//[ test_chain_classes
class UnfusedDoublerClass : public signals::filter<UnfusedDoublerClass, void (float)>
{
public:
    typedef void result_type;
	void operator()(float x) {out(2*x);}
};

class FusedDoublerClass : public signals::fusion_filter<FusedDoublerClass, void (float), mpl::vector<>, signals::output::fused>
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
        signals::storage<void (float), signals::output::fused> floater(1.0f);
        signals::storage<void (float), signals::output::fused> collector(0.0f);
        
        floater >>= doubling_chain >>= collector;
        floater.send();
        
        BOOST_CHECK_EQUAL(collector.at<0>(), 16.0f);
        //]
    }
    return 0;
} // int test_main(int, char* [])
