/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/port_binary_operation_implementation_adapter.hpp>

#include "my_blueprint_ports.hpp"
#include "../generic/my_connect.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace df = boost::dataflow;


BOOST_AUTO_TEST_CASE( test ) 
{
    my_blueprint_port_producer producer;
    my_blueprint_port_consumer consumer;
    
    std::auto_ptr<df::blueprint::port_binary_operation_implementation<my_blueprint_framework> >
        op(new df::blueprint::port_binary_operation_implementation_adapter<my_port_producer, my_port_consumer, connect, my_blueprint_framework>);
}
