// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/stat/bivariate.hpp>
#include <boost/assign/std.hpp>
//remark: como demands this header to be first!!!
#include <boost/test/floating_point_comparison.hpp> 
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>
#include <limits>
#include <iostream>

using namespace boost::assign;
using namespace boost::algorithm;
using boost::unit_test_framework::test_suite;
using namespace std;


void         test1();
test_suite*  init_unit_test_suite( int argc, char* argv[] );
const float  eps = 0.0001f;

void test1()
{
    vector<float> v1, v2, v3;
    v1 += 1,2,3,4,5,6,7,8,9;
    v2 += -1,-2,-3,-4,-5,-6,-7,-8,-9;
    boost::tuple<float,float> means = mean<float>( v1.begin(), v1.end(), v3.begin(), v3.end() );
    const float mean_v1   = 5;
    const float mean_v3   = -5;
    
    BOOST_CHECK_EQUAL( mean_v1, boost::get<0>( means ) );
    BOOST_CHECK_EQUAL( mean_v3, boost::get<1>( means ) );
    const float size      = 8;  // 9 - 1
    const float sum_v1v2  = 60; // (x_i - mean_x)*(y_i - mean_y) 
    const float sum_v1v3  = -60;
    
    float cov1 = covariance<float>( v1.begin(), v1.end(), v1.begin(), v1.end() );
    float res1 = sum_v1v2 / size;
    BOOST_CHECK_EQUAL( int( res1 ), int( cov1 ) );
    BOOST_CHECK_CLOSE( res1, cov1, eps );
    
    float cov2 = covariance<float>( v1.begin(), v1.end(), v3.begin(), v3.end() );
    float res2 = sum_v1v3 / size;
    BOOST_CHECK_EQUAL( int( res2 ) , int( cov2 ) );
    BOOST_CHECK_CLOSE( res2, cov2, eps );
    
    BOOST_CHECK_EQUAL( 1, correlation_coefficient<float>( v1.begin(), v1.end(), v1.begin(), v1.end() ) ); 
    BOOST_CHECK_EQUAL( -1, correlation_coefficient<float>( v1.begin(), v1.end(), v3.begin(), v3.end() ) ); 
    
}

#include <boost/test/included/unit_test_framework.hpp>

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "Univariate test suite" );

    test->add( BOOST_TEST_CASE( &test1 ) );

    return test;
}

