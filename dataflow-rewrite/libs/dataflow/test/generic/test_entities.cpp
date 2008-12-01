/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/generic/entities.hpp>
#include "my_static_vector.hpp"

#include <boost/type_traits/is_same.hpp>


#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE( test )
{
    my_static_vector x(0.5, 2);
    
    typedef df::result_of::entities<my_static_vector>::type result_of_entities;
    
    BOOST_CHECK((boost::is_same<result_of_entities, ::boost::fusion::vector2<float,int> & >::value));
    
    BOOST_CHECK_EQUAL(&df::entities(x), &x.ports);
}