// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/filter.hpp>
#include <boost/dataflow/signals/component/storage.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

template<typename SignalSelector=typename boost::signals::default_signal_args<void(float)>::type>
class generator : public boost::signals::filter<generator<SignalSelector>, void(float), void, SignalSelector>
{
public:
    void send()
    {
        generator::out(1.0f);
    }
};

template<typename SignalSelector=typename boost::signals::default_signal_args<void(float)>::type>
class doubler : public boost::signals::filter<doubler<SignalSelector>, void(float), void, SignalSelector>
{
public:
    void operator()(float x)
    {
        doubler::out(x*2);
    }
};

struct doubler_selector
{
    template<typename OutSignature>
    struct apply;
};

template<>
struct doubler_selector::apply<void(float)>
{
    typedef doubler<> type;
};

struct quadrupler_selector
{
    template<typename OutSignature>
    struct apply;
};

template<>
struct quadrupler_selector::apply<void(float)>
{
    typedef doubler<doubler_selector> type;
};

int test_main(int, char* [])
{
    namespace df = boost::dataflow;
    using namespace boost;

    {
        generator<> g;
        doubler<> d;
        signals::storage<void (float)> collector(0.0f);

        connect(g, d);
        connect(d, collector);
        g.send();
        BOOST_CHECK_EQUAL(collector.at<0>(), 2.0f);
    }
    
    {
        generator<doubler_selector> doubling_g;
        signals::storage<void (float)> collector(0.0f);
 
        connect(doubling_g, collector);
        doubling_g.send();
        BOOST_CHECK_EQUAL(collector.at<0>(), 2.0f);
    }
    
    {
        generator<quadrupler_selector> quad_g;
        signals::storage<void (float)> collector(0.0f);
 
        connect(quad_g, collector);
        quad_g.send();
        BOOST_CHECK_EQUAL(collector.at<0>(), 4.0f);
    }
    
    return 0;
}
