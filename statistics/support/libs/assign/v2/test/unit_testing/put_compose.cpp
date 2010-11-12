//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_ASSIGN_V2_TEST_UNIT_TEST_PUT_COMPOSE_ER_2010_CPP
#define LIBS_ASSIGN_V2_TEST_UNIT_TEST_PUT_COMPOSE_ER_2010_CPP

#include <iostream> // needed?

#include <boost/test/test_tools.hpp>
#define BOOST_ASSIGN_CHECK_EQUAL(a,b) BOOST_CHECK_EQUAL(a,b)
#include <libs/assign/v2/test/put/compose/common/forward_parameter.cpp>
#include <libs/assign/v2/test/put/compose/csv/composer.cpp>
#include <libs/assign/v2/test/put/compose/csv/stl.cpp>
#include <libs/assign/v2/test/put/compose/functor/composer.cpp>
#include <libs/assign/v2/test/put/compose/functor/forward_args.cpp>
#include <libs/assign/v2/test/put/compose/functor/forward_seq_args.cpp>
#include <libs/assign/v2/test/put/compose/functor/stl.cpp>

#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;
test_suite* init_unit_test_suite( int argc, char* argv[] )
{                                            
    test_suite* test = BOOST_TEST_SUITE( "BOOST_ASSIGN_V2" ); 
    using namespace test_assign_v2;  
    {
    	using namespace xxx_put::xxx_compose; 
        {
        	using namespace xxx_common;
			test->add( BOOST_TEST_CASE( &xxx_forward_parameter::test ) );
        }
        {
        	using namespace xxx_csv;
			test->add( BOOST_TEST_CASE( &xxx_composer::test ) );
			test->add( BOOST_TEST_CASE( &xxx_stl::test ) );
        }
        {
        	using namespace xxx_functor;
			test->add( BOOST_TEST_CASE( &xxx_composer::test ) );
			test->add( BOOST_TEST_CASE( &xxx_forward_args::test ) );
			test->add( BOOST_TEST_CASE( &xxx_forward_seq_args::test ) );
			test->add( BOOST_TEST_CASE( &xxx_stl::test ) );
        }
    }
    return test;                             
}                                            

#endif
