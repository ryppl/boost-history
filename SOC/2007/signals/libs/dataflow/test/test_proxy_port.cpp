// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/support/proxy_port.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;

struct incomplete;
struct empty {};

struct my_mechanism;

struct my_producer_traits
    : public df::port_traits<
        my_mechanism,
        df::ports::producer,
        df::concepts::producer>
{};

struct my_producer : public df::port<my_producer_traits>
{};

struct my_proxy_producer
{
public:
    my_producer p;
    
    template<typename Mechanism, typename PortCategory>
    struct dataflow;
};

template<>
struct my_proxy_producer::dataflow<my_mechanism, df::ports::producer>
{
    typedef df::default_proxy_port<
        my_mechanism,
        df::ports::producer,
        my_producer> proxy_port_traits;

    template<typename T>
    static typename boost::dataflow::utility::copy_cv<my_producer, T>::type &
    get_proxied_port(T &t)
    {
        return t.p;
    };
};

struct my_proxy_proxy_producer
{
public:
    my_proxy_producer proxy;
    
    template<typename Mechanism, typename PortCategory>
    struct dataflow;
};

template<>
struct my_proxy_proxy_producer::dataflow<my_mechanism, df::ports::producer>
{
    typedef df::default_proxy_port<
        my_mechanism,
        df::ports::producer,
        my_proxy_producer> proxy_port_traits;

    template<typename T>
    static typename boost::dataflow::utility::copy_cv<my_proxy_producer, T>::type &
    get_proxied_port(T &t)
    {
        return t.proxy;
    };
};


int test_main(int, char* [])
{
    BOOST_CHECK(( df::is_proxy_port_traits<
            df::default_proxy_port<
                my_mechanism,
                df::ports::producer,
                my_producer>
        >::value ));
    
    BOOST_CHECK(( df::is_proxy_port<my_mechanism, df::ports::producer, my_proxy_producer>::value ));

    my_proxy_producer pp;
    BOOST_CHECK_EQUAL(&pp.p, (&df::get_port<my_mechanism, df::ports::producer>(pp)));

    const volatile my_proxy_producer cv_pp(pp);
    BOOST_CHECK_EQUAL(&cv_pp.p, (&df::get_port<my_mechanism, df::ports::producer>(cv_pp)));
        
    BOOST_CHECK(( df::is_proxy_port<my_mechanism, df::ports::producer, my_proxy_proxy_producer>::value ));

    my_proxy_proxy_producer ppp;
    BOOST_CHECK_EQUAL(&ppp.proxy.p, (&df::get_port<my_mechanism, df::ports::producer>(ppp)));

    const volatile my_proxy_proxy_producer cv_ppp(ppp);
    BOOST_CHECK_EQUAL(&cv_ppp.proxy.p, (&df::get_port<my_mechanism, df::ports::producer>(cv_ppp)));
    
    return 0;
} // int test_main(int, char* [])
