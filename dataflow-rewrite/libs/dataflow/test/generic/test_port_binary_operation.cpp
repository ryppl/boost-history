/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include "my_connect.hpp"
#include <boost/dataflow/generic/port/port.hpp>

#include "my_ports.hpp"
#include "my_entity.hpp"

#include <boost/type_traits/is_same.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace df=boost::dataflow;

BOOST_AUTO_TEST_CASE( test ) 
{
    BOOST_CHECK((df::are_port_binary_operable<my_port_producer, my_port_consumer, connect>::value));
    BOOST_CHECK((!df::are_port_binary_operable<my_port_consumer, my_port_producer, connect>::value));
//    BOOST_CHECK((!df::are_port_binary_operable<my_port_producer, my_port_consumer, connect, df::default_framework>::value));
    BOOST_CHECK_EQUAL(
        (df::port_binary_operation_will_succeed<connect>(my_port_consumer(), my_port_producer())),
        0);
    
    BOOST_CHECK((
        boost::is_same<
            df::result_of::port_binary_operation<my_port_producer, my_port_consumer, connect>::type,
            int
        >::value));
    
    BOOST_CHECK_EQUAL(
        (df::port_binary_operation_framework<connect, my_framework, my_framework>(my_port_producer(), my_port_consumer(), connect(1))),
        1);
    BOOST_CHECK_EQUAL(
        (df::port_binary_operation<connect>(my_port_producer(), my_port_consumer(), connect(2))),
        2);
    BOOST_CHECK_EQUAL(
        (df::port_binary_operation_framework<connect, my_framework, my_framework>(my_port_producer(), my_port_consumer())),
        0);
    BOOST_CHECK_EQUAL(
        (df::port_binary_operation<connect>(my_port_producer(), my_port_consumer())),
        0);
    BOOST_CHECK_EQUAL(
        (df::port_binary_operation_will_succeed<connect>(my_port_producer(), my_port_consumer())),
        1);
    BOOST_CHECK_EQUAL(
        (df::port_binary_operation_will_succeed<connect>(my_port_producer(), my_port_consumer(), connect(11))),
        0);
}