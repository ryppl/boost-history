/**
 * Non-intrusive Assignment Library
 * (C) Thorsten Ottosen 2002-2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/assign for documentation.
 *
 */

#include <boost/assign/fixed_size_assigner.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <algorithm>
#include <iterator>

void check_array()
{
    using namespace std;
    using namespace boost;
    using namespace boost::assignment;

    typedef array<float,6> Array;

    Array a = (list_of(1),2,3,4,5,6);
    BOOST_CHECK_EQUAL( a[0], 1 );
    BOOST_CHECK_EQUAL( a[5], 6 );
    assign_all( a )(2)(3)(4)(5)(6)(7);
    BOOST_CHECK_EQUAL( a[0], 2 );
    BOOST_CHECK_EQUAL( a[5], 7 );
    // last element is implicitly 0
    Array a2 = (list_of(1),2,3,4,5);
    BOOST_CHECK_EQUAL( a2[5], 0 );
    // two last elements are implicitly 0
    a2 = (list_of(1),2,3,4);
    BOOST_CHECK_EQUAL( a2[4], 0 );
    BOOST_CHECK_EQUAL( a2[5], 0 );
    // too many arguments
    BOOST_CHECK_THROW( a2 = list_of(1)(2)(3)(4)(5)(6)(7), assignment_exception );
}

#include <boost/test/included/unit_test_framework.hpp> 

using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "List Test Suite" );

    test->add( BOOST_TEST_CASE( &check_array ) );

    return test;
}

