//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_ASSIGN_V2_TEST_UNIT_TEST_DETAIL_ER_2010_CPP
#define LIBS_ASSIGN_V2_TEST_UNIT_TEST_DETAIL_ER_2010_CPP

#include <iostream> // needed?

#include <boost/test/test_tools.hpp>
#define BOOST_ASSIGN_V2_CHECK( p ) BOOST_CHECK( p )
#include <libs/assign/v2/test/detail/functor.cpp>
#include <libs/assign/v2/test/detail/traits.cpp>

#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "BOOST_ASSIGN_V2" );
    using namespace test_assign_v2;
    {
    	using namespace xxx_detail;
		test->add( BOOST_TEST_CASE( &xxx_functor::test ) );
		test->add( BOOST_TEST_CASE( &xxx_traits::test ) );
    }
    return test;
}

#endif
