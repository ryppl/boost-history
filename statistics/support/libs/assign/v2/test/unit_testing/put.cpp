//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_ASSIGN_V2_TEST_UNIT_TEST_PUT_ER_2010_CPP
#define LIBS_ASSIGN_V2_TEST_UNIT_TEST_PUT_ER_2010_CPP

#include <iostream> // needed?

#include <boost/test/test_tools.hpp>
#define BOOST_ASSIGN_CHECK_EQUAL(a,b) BOOST_CHECK_EQUAL(a,b)
#include <libs/assign/v2/test/put/modulo_op.cpp>
#include <libs/assign/v2/test/put/ptr.cpp>
#include <libs/assign/v2/test/put/static.cpp>
#include <libs/assign/v2/test/put/stl.cpp>
#include <libs/assign/v2/test/put/tuple_refs.cpp>

#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;
test_suite* init_unit_test_suite( int argc, char* argv[] )
{                                            
    test_suite* test = BOOST_TEST_SUITE( "BOOST_ASSIGN_V2" ); 
    using namespace test_assign_v2;  
    {
    	using namespace xxx_put; 
		test->add( BOOST_TEST_CASE( &xxx_modulo_op::test ) );
		test->add( BOOST_TEST_CASE( &xxx_ptr::test ) );
		test->add( BOOST_TEST_CASE( &xxx_static::test ) );
		test->add( BOOST_TEST_CASE( &xxx_stl::test ) );
		test->add( BOOST_TEST_CASE( &xxx_tuple_refs::test ) );
    }
    return test;                             
}                                            

#endif
