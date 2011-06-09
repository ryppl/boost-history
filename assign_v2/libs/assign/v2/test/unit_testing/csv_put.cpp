//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_ASSIGN_V2_TEST_UNIT_TEST_CSV_PUT_ER_2010_CPP
#define LIBS_ASSIGN_V2_TEST_UNIT_TEST_CSV_PUT_ER_2010_CPP

#include <boost/test/test_tools.hpp>
#define BOOST_ASSIGN_V2_CHECK( p ) BOOST_CHECK( p )

#include <libs/assign/v2/test/put/csv_put_basic.cpp>
#include <libs/assign/v2/test/put/csv_put_ext.cpp>
#include <libs/assign/v2/test/put/do_csv_put.cpp>

#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "BOOST_ASSIGN_V2" );
    using namespace test_assign_v2;
    {
        namespace ns = xxx_put;
        test->add( BOOST_TEST_CASE( &ns::xxx_csv_put_basic::test ) );
        test->add( BOOST_TEST_CASE( &ns::xxx_csv_put_ext::test ) );
        test->add( BOOST_TEST_CASE( &ns::xxx_do_csv_put::test ) );
    }
    return test;
}

#endif // LIBS_ASSIGN_V2_TEST_UNIT_TEST_CSV_PUT_ER_2010_CPP
