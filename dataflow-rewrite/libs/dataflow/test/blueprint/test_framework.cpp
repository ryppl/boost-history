/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#include "my_blueprint_framework.hpp"

#include <boost/type_traits/is_same.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE( test ) 
{
    BOOST_CHECK((df::is_framework<my_blueprint_framework>::value));
    BOOST_CHECK((boost::is_same<my_blueprint_framework::framework_type, my_framework>::value));    
}
