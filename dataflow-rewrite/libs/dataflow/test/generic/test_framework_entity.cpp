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

struct my_framework : public df::framework
{};

typedef df::traits<df::default_framework> default_traits;
typedef df::traits<my_framework> my_traits;

struct default_entity
{
    typedef default_traits dataflow_traits;
};

struct my_entity
{
    typedef my_traits dataflow_traits;
};

BOOST_AUTO_TEST_CASE( test ) 
{
    BOOST_CHECK(df::is_traits<my_traits>::value);
    BOOST_CHECK((df::is_framework_entity<default_entity, df::default_framework>::value));
    BOOST_CHECK((df::is_framework_entity<my_entity, my_framework>::value));
    BOOST_CHECK((!df::is_framework_entity<default_entity, my_framework>::value));
    BOOST_CHECK((!df::is_framework_entity<my_entity, df::default_framework>::value));
    BOOST_CHECK(df::is_framework_entity<default_entity>::value);
    BOOST_CHECK(df::is_framework_entity<my_entity>::value);
}
