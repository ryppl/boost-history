/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/vivid/factory_window.hpp>
#include <boost/dataflow/vivid/network_window.hpp>
#include "../blueprint/my_blueprint_framework.hpp"
#include "../generic/my_ports.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE( test ) 
{
    df::vivid::factory_window<my_blueprint_framework> window;
    
    df::blueprint::factory<my_blueprint_framework> factory;
    factory.add_port<my_port_producer>("my_port_producer");

    window.set_factory(factory);
}
