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
#include "../generic/my_ports_with_context.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace df = boost::dataflow;


BOOST_AUTO_TEST_CASE( test ) 
{
    my_blueprint_framework_context fo;

    my_port_producer p;
    df::blueprint::framework_entity_adapter<my_blueprint_framework, my_port_producer &> ref_entity(fo, p);
    df::blueprint::framework_entity_adapter<my_blueprint_framework, my_port_producer> composition_entity(fo);
    
    BOOST_CHECK(ref_entity.type_info() == typeid(p));
    BOOST_CHECK_EQUAL(&ref_entity.entity(), &p);
    BOOST_CHECK(composition_entity.type_info() == typeid(p));
    
    my_blueprint_framework_with_context_context fcc;
    my_object context;
    my_port_with_context pwc(context);
    
    // test copy construction
    df::blueprint::framework_entity_adapter<my_blueprint_framework_with_context, my_port_with_context> context_copy_construction(fcc, pwc);
}
