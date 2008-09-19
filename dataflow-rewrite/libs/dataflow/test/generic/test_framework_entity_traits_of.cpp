/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/generic/framework_entity/traits_of.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace df = boost::dataflow;

struct my_framework : public df::framework_base
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

BOOST_DATAFLOW_TRAITS(my_entity_nonintrusive, my_traits)

struct my_entity_nonintrusive_enabled
{
};

BOOST_DATAFLOW_TRAITS_ENABLE_IF(
    T,
    is_same<T BOOST_PP_COMMA() my_entity_nonintrusive_enabled>,
    my_traits)


BOOST_AUTO_TEST_CASE( test ) 
{
    // specifying framework explicitly
    BOOST_CHECK((boost::is_same<
        df::traits_of<default_entity_intrusive, df::default_framework>::type,
        default_traits>::value
    ));
    BOOST_CHECK((boost::is_same<
        df::traits_of<my_entity_intrusive, my_framework>::type,
        my_traits>::value
    ));
    BOOST_CHECK((boost::is_same<
        df::traits_of<my_entity_nonintrusive, my_framework>::type,
        my_traits>::value
    ));
    BOOST_CHECK((boost::is_same<
        df::traits_of<my_entity_nonintrusive_enabled, my_framework>::type,
        my_traits>::value
    ));

    // default framework deduction
    BOOST_CHECK((boost::is_same<
        df::traits_of<my_entity_intrusive>::type,
        my_traits>::value
    ));
}
