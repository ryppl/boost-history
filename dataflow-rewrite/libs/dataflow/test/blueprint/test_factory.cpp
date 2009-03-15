/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/factory.hpp>

#include "my_blueprint_framework.hpp"
#include "../generic/my_ports.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace df = boost::dataflow;

BOOST_AUTO_TEST_CASE( test_default ) 
{
    typedef df::blueprint::factory<my_blueprint_framework> factory_type;
    factory_type factory;
    factory.add<factory_type::port_adapter_selector, my_port_producer>("my_port_producer");
    
    df::blueprint::framework_context<my_blueprint_framework> fo;
    df::blueprint::framework_entity<my_blueprint_framework> *entity = factory["my_port_producer"](fo);
    
    BOOST_CHECK_EQUAL(&entity->framework_context(), &fo);
    BOOST_CHECK(entity->type_info() == typeid(my_port_producer));
}

typedef df::blueprint::factory<my_blueprint_framework, boost::mpl::vector<int> > factory_int_type;

struct constructor_int
{
    factory_int_type::produced_type *operator()(factory_int_type::framework_context_type &c, int x) const
    {
        return new df::blueprint::port_adapter<my_blueprint_framework, df::utility::containing_ptr<my_port_producer> >(c);
    }
};

BOOST_AUTO_TEST_CASE( test_signature ) 
{
    df::blueprint::factory<my_blueprint_framework, boost::mpl::vector<int> > factory;
    factory.add_entity("my_port_producer_int", constructor_int());
    
    df::blueprint::framework_context<my_blueprint_framework> fo;
    df::blueprint::framework_entity<my_blueprint_framework> * entity = factory["my_port_producer_int"](fo, 1);
    entity->type_info();
}