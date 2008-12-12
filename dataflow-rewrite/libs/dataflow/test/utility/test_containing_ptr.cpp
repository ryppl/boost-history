/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/utility/containing_ptr.hpp>

#include <boost/type_traits/is_same.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

struct t
{
    void method()
    {};
};

BOOST_AUTO_TEST_CASE( test ) 
{
    using namespace boost;
    using boost::dataflow::utility::containing_ptr;

    containing_ptr<int> five(5);
    
    BOOST_CHECK((is_same<pointee<containing_ptr<int> >::type, int>::value));
    
    BOOST_CHECK_EQUAL(*five, 5);
    
    containing_ptr<t> to;
    to->method();
}
