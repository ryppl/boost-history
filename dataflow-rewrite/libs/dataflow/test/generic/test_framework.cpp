/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include "my_framework.hpp"
#include "my_framework_with_context.hpp"

#include <boost/type_traits/is_same.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace df = boost::dataflow;

struct not_framework
{};

BOOST_AUTO_TEST_CASE( test ) 
{
    BOOST_CHECK((df::is_framework<my_framework>::value));
    BOOST_CHECK((!df::has_framework_context<my_framework>::value));
    
    BOOST_CHECK((!df::is_framework<not_framework>::value));

    BOOST_CHECK((df::is_framework<my_framework_with_context>::value));
    BOOST_CHECK((df::has_framework_context<my_framework_with_context>::value));
    BOOST_CHECK((boost::is_same<my_framework_with_context::framework_context_type, my_object>::value));
}



