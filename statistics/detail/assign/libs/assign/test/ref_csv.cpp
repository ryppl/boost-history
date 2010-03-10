//////////////////////////////////////////////////////////////////////////////
// assign::test::ref_csv.cpp                                                //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#  pragma warn -8091 // supress warning in Boost.Test
#  pragma warn -8057 // unused argument argc/argv in Boost.Test
#endif

#include <boost/test/test_tools.hpp>
#define BOOST_ASSIGN_CHECK_EQUAL(a,b) BOOST_CHECK_EQUAL(a,b)
#include <boost/assign/auto_size/check/ref_csv.hpp>

void check_ref_csv()
{
    boost::assign::detail::auto_size::check_ref_csv<int>();      
}
#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "List Test Suite" );

    test->add( BOOST_TEST_CASE( &check_ref_csv ) );

    return test;
}

#undef BOOST_ASSIGN_CHECK_EQUAL
