/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/port_binary_operation.hpp>
#include <boost/dataflow/blueprint/operation_adapter.hpp>

#include "my_blueprint_ports.hpp"
#include "../generic/my_connect.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace df = boost::dataflow;


BOOST_AUTO_TEST_CASE( test ) 
{
    my_blueprint_framework_object fo;

    my_blueprint_port_producer producer(fo);
    my_blueprint_port_consumer consumer(fo);
    df::blueprint::operation_adapter<connect> blueprint_connect;

    BOOST_CHECK(( df::are_port_binary_operable<my_blueprint_port_producer, my_blueprint_port_consumer, df::blueprint::operation>::value ));
    BOOST_CHECK(( df::are_port_binary_operable<my_blueprint_port_consumer, my_blueprint_port_producer, df::blueprint::operation>::value ));

    // will not succeed without the operation being added to the framework object.
    BOOST_CHECK(( !df::port_binary_operation_will_succeed<df::blueprint::operation>(producer, consumer, blueprint_connect) ));
    BOOST_CHECK(( !df::port_binary_operation_will_succeed<df::blueprint::operation>(consumer, producer, blueprint_connect) ));
}
