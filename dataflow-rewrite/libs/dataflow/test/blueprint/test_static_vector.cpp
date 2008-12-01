/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include "my_blueprint_static_vector.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace df = boost::dataflow;

BOOST_AUTO_TEST_CASE( test ) 
{
    my_blueprint_framework_context fo;
    my_blueprint_static_vector vector(fo, my_static_vector(2.5, 1));
    df::blueprint::port_iterator<my_blueprint_framework> it = vector.begin();
    BOOST_CHECK_EQUAL(it->get_as<float>(), 2.5);
    ++it;
    BOOST_CHECK_EQUAL(it->get_as<int>(), 1);
    ++it;
    BOOST_CHECK(it == vector.end());
}