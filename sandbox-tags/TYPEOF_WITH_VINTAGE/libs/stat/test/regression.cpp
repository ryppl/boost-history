// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/stat/regression.hpp>
#include <boost/assign/std.hpp>
//remark: como demands this header to be first!!!
#include <boost/test/floating_point_comparison.hpp> 
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>
#include <limits>
#include <cmath>

using namespace boost::assign;
using namespace boost::algorithm;
using boost::unit_test_framework::test_suite;
using namespace std;


void         test1();
test_suite*  init_unit_test_suite( int argc, char* argv[] );
const float  eps = 0.0001f;

template< class T >
struct identity_transform
{
    typedef T result_type;
    T operator()( T t ) const
    {
        return t;
    }
};


void test1()
{
    vector<float> v1, v2, v3, v4;
    v1 += 1,2,3,4,5,6,7,8,9;
    v2 += -1,-2,-3,-4,-5,-6,-7,-8,-9;
    v3 += 1,4,9,16,25,36,49,64,81;    // x^2
    v4 += 2,4,8,16,32,64,128,256,512; // 2^x

    //
    // do regression for y = ax + b
    //
    float a,b,corr;

    // x = v1, y = v1
    boost::tie( a, b, corr ) = least_square_line<float>( v1.begin(), v1.end(), v1.begin(), v1.end() );
    BOOST_CHECK_EQUAL( a, 1 );
    BOOST_CHECK_EQUAL( b, 0 );
    BOOST_CHECK_EQUAL( corr, 1 );
         
    // x = v1, y = v2
    boost::tie( a, b, corr ) = least_square_line<float>( v1.begin(), v1.end(), v2.begin(), v2.end() );
    BOOST_CHECK_EQUAL( a, -1 );
    BOOST_CHECK_EQUAL( b, 0 );
    BOOST_CHECK_EQUAL( corr, -1 );
    
    // x = v1, y = v3
    boost::tie( a, b, corr ) = least_square_line<float>( v1.begin(), v1.end(), v3.begin(), v3.end() );
    BOOST_CHECK( a != 1 );
    BOOST_CHECK( b != 0 );
    BOOST_CHECK( corr != 1 );

    // x = log( v1 ), y = log( v3 )
    boost::tie( a, b, corr ) = least_square_line<float>( v1.begin(), v1.end(), &log10, 
                                                         v3.begin(), v3.end(), &log10 );
    BOOST_CHECK_EQUAL( a, 2 );
    BOOST_CHECK_EQUAL( b, 0 );
    BOOST_CHECK_EQUAL( corr, 1 );

    // y = log( v4 ), x = v1
    // we would like to measure exponential relation, but because we need to
    // transform the only second coordinate, we must reverse the arguments 
    // or provide an identity transformation on x. 
    
    // method 1: apply identity transformation on x
    boost::tie( a, b, corr ) = least_square_line<float>( v1.begin(), v1.end(), identity_transform<int>(), 
                                                         v4.begin(), v4.end(), &log10 );
    const float err = 0.00001f;
    BOOST_CHECK_CLOSE( a, log10( 2.f ), eps );
    BOOST_CHECK( b > -err && b < err );
    BOOST_CHECK_EQUAL( corr, 1 );

    // method 2: swap x and y arguments
    boost::tie( a, b, corr ) = least_square_line<float>( v4.begin(), v4.end(), &log10, 
                                                         v1.begin(), v1.end() );
    BOOST_CHECK_CLOSE( a, 1 / log10( 2.f ), eps );
    BOOST_CHECK( b > -err && b < err );
    BOOST_CHECK_EQUAL( corr, 1 );
/*
    boost::tie( a, b, corr ) = least_square_line<float>( make_transform_iterator( v4.begin(), &log10 ), 
                                                         make_transform_iterator( v4.end(), &log10 ), 
                                                         v1.begin(), v1.end() );    

*/
    
}


#include <boost/test/included/unit_test_framework.hpp>

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "Regression test suite" );

    test->add( BOOST_TEST_CASE( &test1 ) );

    return test;
}




