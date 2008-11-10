/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/framework_entity.hpp>
#include <boost/dataflow/blueprint/framework_entity_adapter.hpp>

#include "my_blueprint_framework.hpp"
#include "../generic/my_ports.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace df = boost::dataflow;


BOOST_AUTO_TEST_CASE( test ) 
{
    my_blueprint_framework_object fo;

    my_port_producer p;
    df::blueprint::framework_entity_adapter<my_blueprint_framework, my_port_producer &> ref_entity(fo, p);
    df::blueprint::framework_entity_adapter<my_blueprint_framework, my_port_producer> composition_entity(fo);
    
    BOOST_CHECK(ref_entity.type_info() == typeid(p));
    BOOST_CHECK(composition_entity.type_info() == typeid(p));
}
