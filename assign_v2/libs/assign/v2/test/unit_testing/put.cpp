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

#include <boost/test/test_tools.hpp>
#define BOOST_ASSIGN_V2_CHECK( p ) BOOST_CHECK( p )

#include <libs/assign/v2/test/value/deque.cpp>
#include <libs/assign/v2/test/value/fun.cpp>
#include <libs/assign/v2/test/value/modifier/deduce.cpp>
#include <libs/assign/v2/test/value/modifier/iterate.cpp>
#include <libs/assign/v2/test/value/modifier/lookup.cpp>
#include <libs/assign/v2/test/value/modifier/repeat.cpp>
#include <libs/assign/v2/test/value/modifier/standard.cpp>
#include <libs/assign/v2/test/value/pipe/args_list.cpp>
#include <libs/assign/v2/test/value/pipe/fun.cpp>
#include <libs/assign/v2/test/value/pipe/modifier/iterate.cpp>
#include <libs/assign/v2/test/value/pipe/modifier/lookup.cpp>
#include <libs/assign/v2/test/value/pipe/modifier/repeat.cpp>
#include <libs/assign/v2/test/value/pipe/modifier/standard.cpp>
#include <libs/assign/v2/test/value/pipe/put.cpp>
#include <libs/assign/v2/test/value/ptr.cpp>
#include <libs/assign/v2/test/value/put.cpp>


#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "BOOST_ASSIGN_V2" );
    using namespace test_assign_v2;
    {

    	namespace ns = xxx_value;
		test->add( BOOST_TEST_CASE( &ns::xxx_deque::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_fun::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_modifier::xxx_deduce::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_modifier::xxx_iterate::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_modifier::xxx_lookup::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_modifier::xxx_repeat::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_modifier::xxx_standard::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_args_list::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_fun::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_modifier::xxx_iterate::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_modifier::xxx_lookup::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_modifier::xxx_repeat::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_modifier::xxx_standard::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_put::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_ptr::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_put::test ) );
    }
    return test;
}

#endif
