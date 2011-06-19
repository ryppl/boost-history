///////////////////////////////////////////////////////////////////////////////
//  acc_ecdf                                                                 //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <boost/test/test_tools.hpp>
#define BOOST_ACCUMULATORS_ECDF_CHECK( p ) BOOST_CHECK( p )
#include <libs/accumulators/ecdf/test/ecdf.cpp>
#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "ECDF" );
    test->add( BOOST_TEST_CASE( &test_ecdf ) );
    return test;
}


