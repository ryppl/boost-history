/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/operation_adapter.hpp>

#include "../generic/my_connect.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace df = boost::dataflow;


BOOST_AUTO_TEST_CASE( test ) 
{
    df::blueprint::operation_adapter<connect> c;
    df::blueprint::operation_adapter<connect> c10(10);    
}
