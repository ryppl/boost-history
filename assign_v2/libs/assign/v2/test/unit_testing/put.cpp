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
#define BOOST_ASSIGN_V2_CHECK( p ) BOOST_CHECK( p )

#include <libs/assign/v2/test/put/deduce.cpp>

#include <libs/assign/v2/test/put/container/functor.cpp>
#include <libs/assign/v2/test/put/container/range.cpp>
#include <libs/assign/v2/test/put/deque/functor.cpp>

#include <libs/assign/v2/test/put/modulo/fun.cpp>
#include <libs/assign/v2/test/put/modulo/std.cpp>
#include <libs/assign/v2/test/put/modulo/ext.cpp>

#include <libs/assign/v2/test/put/ptr.cpp>

#include <libs/assign/v2/test/put/pipe/functor/forward.cpp>
#include <libs/assign/v2/test/put/pipe/functor/rhs.cpp>
#include <libs/assign/v2/test/put/pipe/functor/str_literal.cpp>
#include <libs/assign/v2/test/put/pipe/container.cpp>
#include <libs/assign/v2/test/put/pipe/modulo/forward.cpp>
#include <libs/assign/v2/test/put/pipe/modulo/fun.cpp>
#include <libs/assign/v2/test/put/pipe/modulo/std.cpp>
#include <libs/assign/v2/test/put/pipe/range.cpp>

#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "BOOST_ASSIGN_V2" );
    using namespace test_assign_v2;
    {

    	namespace ns = xxx_put;
		test->add( BOOST_TEST_CASE( &ns::xxx_deduce::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_container::xxx_functor::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_container::xxx_range::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_deque::xxx_functor::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_modulo::xxx_fun::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_modulo::xxx_std::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_modulo::xxx_ext::test ) );

		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_functor::xxx_forward::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_functor::xxx_rhs::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_functor::xxx_str_literal::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_container::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_modulo::xxx_fun::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_modulo::xxx_std::test ) );
		test->add( BOOST_TEST_CASE( &ns::xxx_pipe::xxx_range::test ) );

    }
    return test;
}

#endif
