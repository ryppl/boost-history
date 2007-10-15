// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/support/port.hpp>

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

struct my_consumer_traits
    : public df::port_traits<
        my_mechanism,
        df::ports::consumer,
        df::concepts::consumer>
{};

struct my_producer : public df::port<my_producer_traits>
{};

struct my_consumer : public df::port<my_consumer_traits>
{};

int test_main(int, char* [])
{
    BOOST_CHECK(df::is_port_category<df::ports::producer>::value);
    BOOST_CHECK(df::is_port_category<df::ports::consumer>::value);
    
    BOOST_CHECK(!df::is_port_category<incomplete>::value);
    BOOST_CHECK(!df::is_port_category<empty>::value);
    
    BOOST_CHECK(df::is_port_traits<my_producer_traits>::value);
    BOOST_CHECK(df::is_port_traits<my_consumer_traits>::value);
    
    BOOST_CHECK(!df::is_port_traits<incomplete>::value);
    BOOST_CHECK(!df::is_port_traits<empty>::value);
    
    BOOST_CHECK(( df::is_port<my_mechanism, df::ports::producer, my_producer>::value ));
    BOOST_CHECK(( df::is_port<my_mechanism, df::ports::consumer, my_consumer>::value ));
    
    BOOST_CHECK(( !df::is_port<my_mechanism, df::ports::producer, incomplete>::value ));
    BOOST_CHECK(( !df::is_port<my_mechanism, df::ports::producer, empty>::value ));
    
    my_producer p;
    
    BOOST_CHECK_EQUAL(&p, (&df::get_port<my_mechanism, df::ports::producer>(p) ));
    BOOST_CHECK(( boost::is_same<df::get_port_result_type<my_mechanism, df::ports::producer, my_producer>::type, my_producer &>::value ));
    
    const volatile my_producer cp(p);
    
    BOOST_CHECK_EQUAL(&cp, (&df::get_port<my_mechanism, df::ports::producer>(cp)));
    BOOST_CHECK(( boost::is_same<df::get_port_result_type<my_mechanism, df::ports::producer, const volatile my_producer>::type, const volatile my_producer &>::value ));
    
    return 0;
} // int test_main(int, char* [])
