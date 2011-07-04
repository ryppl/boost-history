///////////////////////////////////////////////////////////////////////////////
//  acc_ecdf                                                                 //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <boost/test/test_tools.hpp>
#define BOOST_ACCUMULATORS_ECDF_CHECK( p ) BOOST_CHECK( p )
#include <libs/accumulators/ecdf/test/count.cpp>
#include <libs/accumulators/ecdf/test/cumulative_count.cpp>
#include <libs/accumulators/ecdf/test/pdf.cpp>
#include <libs/accumulators/ecdf/test/cdf.cpp>
#include <libs/accumulators/ecdf/test/kolmogorov_smirnov_statistic.cpp>

#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "ECDF" );
    test->add( BOOST_TEST_CASE( &test_ecdf_count ) );
    test->add( BOOST_TEST_CASE( &test_ecdf_cumulative_count ) );
    test->add( BOOST_TEST_CASE( &test_ecdf_pdf ) );
    test->add( BOOST_TEST_CASE( &test_ecdf_cdf ) );
    test->add( BOOST_TEST_CASE( &test_ecdf_kolmogorov_smirnov_statistic ) );
    return test;
}

