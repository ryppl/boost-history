/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/framework_object.hpp>

#include "my_blueprint_framework.hpp"
#include "../generic/my_connect.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace df = boost::dataflow;


BOOST_AUTO_TEST_CASE( test ) 
{
    df::blueprint::framework_object<my_blueprint_framework> object;
    object.register_operation<my_port_producer, my_port_consumer, connect>();
    BOOST_CHECK((object.has_registered_operation<my_port_producer, my_port_consumer, connect>()));
    BOOST_CHECK((!object.has_registered_operation<my_port_consumer, my_port_producer, connect>()));    
}
