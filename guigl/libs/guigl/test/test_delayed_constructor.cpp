/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/parameter/aux_/delayed_constructor.hpp>
#include <utility>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test )
{
    using namespace boost::parameter::aux;

    typedef delayed_constructor<int, int> delayed_int_constructor_type;
    delayed_int_constructor_type delayed_int(2);
    int i = delayed_int;
    
    BOOST_CHECK_EQUAL(i, 2);

    
    delayed_constructor<std::pair<int, double>, int, double> delayed(1, 3.5);
    
    std::pair<int, double> p = delayed;
    
    BOOST_CHECK_EQUAL(p.first, 1);
    BOOST_CHECK_EQUAL(p.second, 3.5);
    
    BOOST_CHECK(is_delayed_constructor<delayed_int_constructor_type>::value);
}