//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_ASSIGN_V2_TEST_UNIT_TEST_REF_ER_2010_CPP
#define LIBS_ASSIGN_V2_TEST_UNIT_TEST_REF_ER_2010_CPP

#include <iostream> // needed?

#include <boost/test/test_tools.hpp>
#define BOOST_ASSIGN_V2_CHECK( p ) BOOST_CHECK( p )
#include <libs/assign/v2/test/ref/convert_traits.cpp>
#include <libs/assign/v2/test/ref/wrapper.cpp>
#include <libs/assign/v2/test/ref/list.cpp>
#include <libs/assign/v2/test/ref/list_tuple.cpp>
#include <libs/assign/v2/test/ref/array/functor.cpp>
#include <libs/assign/v2/test/ref/array/csv.cpp>

#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "BOOST_ASSIGN_V2" );
    using namespace test_assign_v2;
    {
    	using namespace xxx_ref;
		test->add( BOOST_TEST_CASE( &xxx_convert_traits::test ) );
		test->add( BOOST_TEST_CASE( &xxx_wrapper::test ) );
		test->add( BOOST_TEST_CASE( &xxx_list::test ) );
		test->add( BOOST_TEST_CASE( &xxx_list_tuple::test ) );
		test->add( BOOST_TEST_CASE( &xxx_array::xxx_functor::test ) );
		test->add( BOOST_TEST_CASE( &xxx_array::xxx_csv::test ) );
    }
    return test;
}

#endif
