/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/generic/framework_entity.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace df = boost::dataflow;

typedef df::traits<df::default_framework> my_traits;

BOOST_AUTO_TEST_CASE( test ) 
{
    BOOST_CHECK(df::is_traits<my_traits>::value);
}
