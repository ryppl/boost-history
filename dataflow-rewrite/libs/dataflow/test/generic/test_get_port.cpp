/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/generic/static_vector/get_port.hpp>
#include "my_static_vector.hpp"

#include <boost/type_traits/is_same.hpp>


#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE( test )
{
    tuple x(0.5, 2);
    
    typedef df::result_of::get_port<tuple, boost::mpl::int_<0> >::type result_of_0;
    typedef df::result_of::get_port<tuple, boost::mpl::int_<1> >::type result_of_1;
    
    BOOST_CHECK((boost::is_same<result_of_0, float>::value));
    BOOST_CHECK((boost::is_same<result_of_1, int>::value));
    
    BOOST_CHECK_EQUAL((df::get_port(x, boost::mpl::int_<0>())), 0.5);
    BOOST_CHECK_EQUAL((df::get_port<boost::mpl::int_<0> >(x)), 0.5);

    BOOST_CHECK_EQUAL((df::get_port(x, boost::mpl::int_<1>())), 2);
    BOOST_CHECK_EQUAL((df::get_port<boost::mpl::int_<1> >(x)), 2);
}