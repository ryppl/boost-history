// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/stat/multivariate.hpp>
#include <boost/assign/ublas.hpp>
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
namespace    u   = boost::numeric::ublas;

void test1()
{
    typedef u::vector<float> vector_t;
    typedef u::matrix<float> matrix_t;
    const int rows = 4, cols = 4;
    
    matrix_t m( rows, cols );
    m <<   1,2,3,4,
           1,2,3,4,
           2,2,2,2,
           4,4,4,4;
    
    
    vector_t mu( rows );
    mu = row_mean< vector_t >( m );
}


#include <boost/test/included/unit_test_framework.hpp>

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "Univariate test suite" );

    test->add( BOOST_TEST_CASE( &test1 ) );

    return test;
}

