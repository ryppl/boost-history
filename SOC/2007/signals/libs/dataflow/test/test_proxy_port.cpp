// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "my_producer_consumer.hpp"
#include <boost/dataflow/support/proxy_port.hpp>

#include <boost/test/included/test_exec_monitor.hpp>


struct my_proxy_producer
{
public:
    my_producer p;
        
    struct get_my_producer
    {
        template<typename T>
        typename boost::dataflow::utility::copy_cv<my_producer, T>::type &
        operator()(T &t)
        {
            return t.p;
        }
    };
    
    typedef df::default_proxy_port<
        my_mechanism,
        df::ports::producer,
        my_producer,
        get_my_producer
        > proxy_port_traits;
};

struct my_proxy_proxy_producer
{
public:
    my_proxy_producer proxy;

    struct get_my_proxy_producer
    {
        template<typename T>
        typename boost::dataflow::utility::copy_cv<my_proxy_producer, T>::type &
        operator()(T &t)
        {
            return t.proxy;
        }
    };
    
    typedef df::default_proxy_port<
        my_mechanism,
        df::ports::producer,
        my_proxy_producer,
        get_my_proxy_producer> proxy_port_traits;
};

int test_main(int, char* [])
{
    BOOST_CHECK(( df::is_proxy_port_traits<
            df::default_proxy_port<
                my_mechanism,
                df::ports::producer,
                my_producer,
                my_proxy_producer::get_my_producer
                >
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
