/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/generic/framework_entity/default_framework_of.hpp>
#include <boost/dataflow/generic/framework_entity/traits.hpp>

#include <boost/preprocessor/comma.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace df = boost::dataflow;

struct my_framework : public df::framework_base
{};

struct no_traits
{};

typedef df::traits<df::default_framework> default_traits;
typedef df::traits<my_framework> my_traits;

struct default_entity_intrusive
{
    typedef default_traits dataflow_traits;
}; 

struct my_entity_intrusive
{
    typedef my_traits dataflow_traits;
}; 

struct my_entity_nonintrusive
{
};

BOOST_DATAFLOW_DEFAULT_FRAMEWORK(my_entity_nonintrusive, my_framework)

struct my_entity_nonintrusive_enabled
{
};

BOOST_DATAFLOW_DEFAULT_FRAMEWORK_ENABLE_IF(
    T,
    is_same<T BOOST_PP_COMMA() my_entity_nonintrusive_enabled>,
    my_framework)


BOOST_AUTO_TEST_CASE( test ) 
{
    BOOST_CHECK((boost::is_same<
        df::default_framework_of<no_traits>::type,
        df::default_framework>::value
    ));
    BOOST_CHECK((boost::is_same<
        df::default_framework_of<default_entity_intrusive>::type,
        df::default_framework>::value
    ));
    BOOST_CHECK((boost::is_same<
        df::default_framework_of<my_entity_intrusive>::type,
        my_framework>::value
    ));
    BOOST_CHECK((boost::is_same<
        df::default_framework_of<my_entity_nonintrusive>::type,
        my_framework>::value
    ));
    BOOST_CHECK((boost::is_same<
        df::default_framework_of<my_entity_nonintrusive_enabled>::type,
        my_framework>::value
    ));
}
