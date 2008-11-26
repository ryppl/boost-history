/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include "my_dynamic_vector.hpp"
#include <boost/type_traits/is_same.hpp>


#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace df=boost::dataflow;


BOOST_AUTO_TEST_CASE( test )
{
    my_dynamic_vector v;
    BOOST_CHECK_EQUAL(&df::entities(v), &v.ports);
}