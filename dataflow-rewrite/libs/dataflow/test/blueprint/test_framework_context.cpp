/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/framework_context.hpp>
#include <boost/dataflow/blueprint/operation_adapter.hpp>
#include <boost/dataflow/blueprint/port_binary_operation.hpp>

#include "my_blueprint_ports.hpp"
#include "../generic/my_connect.hpp"

#include "../generic/my_framework_with_context.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace df = boost::dataflow;


BOOST_AUTO_TEST_CASE( test ) 
{
    df::blueprint::framework_context<my_blueprint_framework> object;
    
    object.register_operation<my_port_producer, my_port_consumer, connect>();
    BOOST_CHECK((object.has_registered_operation<my_port_producer, my_port_consumer, connect>()));
    BOOST_CHECK((!object.has_registered_operation<my_port_consumer, my_port_producer, connect>()));
    
    my_blueprint_port_producer producer(object);
    my_blueprint_port_consumer consumer(object);
    df::blueprint::operation_adapter<connect> blueprint_connect(connect(1));

    BOOST_CHECK(( df::port_binary_operation_will_succeed<df::blueprint::operation>(producer, consumer, blueprint_connect) ));
    BOOST_CHECK(( !df::port_binary_operation_will_succeed<df::blueprint::operation>(consumer, producer, blueprint_connect) ));
    BOOST_CHECK_EQUAL( boost::any_cast<int>(df::port_binary_operation<df::blueprint::operation>(producer, consumer, blueprint_connect)), 1);
}

BOOST_AUTO_TEST_CASE( test_framework_with_object ) 
{
    typedef df::blueprint::framework<my_framework_with_context> my_blueprint_framework_with_object;
    
    df::blueprint::framework_context<my_blueprint_framework_with_object> object;

    object.object();
}


