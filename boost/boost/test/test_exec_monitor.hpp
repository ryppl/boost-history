//  (C) Copyright Gennadiy Rozental 2001-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : Entry point for the end user into the Test Execution Monitor.
// ***************************************************************************

#ifndef BOOST_TEST_EXEC_MONITOR_HPP_071894GER
#define BOOST_TEST_EXEC_MONITOR_HPP_071894GER

// Boost.Test
#include <boost/test/test_tools.hpp>

//____________________________________________________________________________//

// ************************************************************************** //
// **************                 Auto Linking                 ************** //
// ************************************************************************** //

// Automatically link to the correct build variant where possible. 
#if !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_TEST_NO_LIB) && !defined(BOOST_TEST_SOURCE)
// If we're importing code from a dll, then tell auto_link.hpp about it:
#  if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_TEST_DYN_LINK)
#    define BOOST_DYN_LINK
#  endif

#  ifndef BOOST_DYN_LINK
#    define BOOST_LIB_NAME boost_test_exec_monitor
#  else
#    define BOOST_LIB_NAME boost_unit_test_framework
#  endif

#  include <boost/config/auto_link.hpp>

#endif  // auto-linking disabled

#ifdef BOOST_TEST_DYN_LINK

#include <boost/bind.hpp>
#include <boost/test/framework.hpp>
#include <boost/test/unit_test_suite.hpp>

// ************************************************************************** //
// **************                     dll_main                 ************** //
// ************************************************************************** //

namespace boost { namespace unit_test {

int BOOST_TEST_DECL dll_main( bool (*init_unit_test_func)(), int argc, char* argv[] );

}}

// ************************************************************************** //
// **************        main function for tests using dll     ************** //
// ************************************************************************** //

int test_main( int argc, char* argv[] );    // prototype for user's test_main()

namespace {

bool
init_test_program()
{
    using namespace ::boost::unit_test::framework;

    master_test_suite().p_name.value = "Test Program";

    master_test_suite().add( 
        BOOST_TEST_CASE( boost::bind( &test_main, master_test_suite().argc, master_test_suite().argv ) ) );

    return true;
}

} // local namespace

int BOOST_TEST_CALL_DECL
main( int argc, char* argv[] )
{
    return ::boost::unit_test::dll_main( &init_test_program, argc, argv );
}

//____________________________________________________________________________//

#endif // BOOST_TEST_DYN_LINK

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.1  2005/12/14 05:42:08  rogeeff
//  components primary headers
//
// ***************************************************************************

#endif // BOOST_TEST_EXEC_MONITOR_HPP_071894GER
