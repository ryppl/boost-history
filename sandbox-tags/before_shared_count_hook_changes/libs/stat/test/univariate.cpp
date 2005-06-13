// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/stat/distribution.hpp>
#include <boost/stat/univariate.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>
#include <cmath>


using boost::unit_test_framework::test_suite;
using namespace boost::algorithm;
using namespace std;

void         test1();
test_suite*  init_unit_test_suite( int argc, char* argv[] );



void test1()
{

    std::vector<int> v;
    v.push_back( 1 );
    v.push_back( 2 );
    v.push_back( 3 );
    
    BOOST_CHECK_EQUAL( 2, mean<float>( v.begin(), v.end() ) );   
    BOOST_CHECK_EQUAL( 1, variance<float>( v.begin(), v.end() ) );
    BOOST_CHECK_EQUAL( 1, std_deviation<float>( v.begin(), v.end() ) );
    BOOST_CHECK_EQUAL( 1, factorial( 0 ) );
    BOOST_CHECK_EQUAL( 1, factorial( 1 ) );
    BOOST_CHECK_EQUAL( 2, factorial( 2 ) );
    BOOST_CHECK_EQUAL( 6, factorial( 3 ) );
    BOOST_CHECK_EQUAL( 0, nPr( 0, 1 ) );
    BOOST_CHECK_EQUAL( 1, nPr( 1, 0 ) );
    BOOST_CHECK_EQUAL( 1, nPr( 1, 1 ) ); 
    BOOST_CHECK_EQUAL( 6, nPr( 3, 2 ) );
    BOOST_CHECK_EQUAL( 6, nPr( 3, 3 ) );
    BOOST_CHECK_EQUAL( 30, nPr( 6, 2 ) );
    BOOST_CHECK_EQUAL( 0, nCr( 0, 1 ) );
    BOOST_CHECK_EQUAL( 1, nCr( 1, 0 ) );
    BOOST_CHECK_EQUAL( 1, nCr( 1, 1 ) );
    BOOST_CHECK_EQUAL( 3, nCr( 3, 2 ) ); 
    
    const float p = 0.5f;
    // toss a fair coin 6 times; what's the probability of 2 heads?
    BOOST_CHECK_EQUAL( binomial( p, 6, 2 ), 15.f/64 ); 
    BOOST_CHECK_EQUAL( binomial( p, 0, 0 ), 0 );
    // toss a fair coin; what's the probability of 2nd head shows in 4'th trial?
    BOOST_CHECK_EQUAL( negative_binomial( p, 4, 2 ), 3.f/16 );
    BOOST_CHECK_EQUAL( negative_binomial( p, 4, 3 ), 3.f/16 );
    //BOOST_CHECK_EQUAL( negative_binomial( p, 0, 0 ), 0 ); 
}

#include <boost/test/included/unit_test_framework.hpp>

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "Univariate test suite" );

    test->add( BOOST_TEST_CASE( &test1 ) );

    return test;
}
