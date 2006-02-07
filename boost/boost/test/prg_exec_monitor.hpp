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
//  Description : Entry point for the end user into the Program Execution Monitor.
// ***************************************************************************

#ifndef BOOST_PRG_EXEC_MONITOR_HPP_071894GER
#define BOOST_PRG_EXEC_MONITOR_HPP_071894GER

#include <boost/test/detail/config.hpp>

//____________________________________________________________________________//

// ************************************************************************** //
// **************                 Auto Linking                 ************** //
// ************************************************************************** //

// Automatically link to the correct build variant where possible. 
#if !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_TEST_NO_LIB) && \
    !defined(BOOST_TEST_SOURCE) && !defined(BOOST_TEST_INCLUDED)
#  define BOOST_LIB_NAME boost_prg_exec_monitor

// If we're importing code from a dll, then tell auto_link.hpp about it:
#  if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_TEST_DYN_LINK)
#    define BOOST_DYN_LINK
#  endif

#  include <boost/config/auto_link.hpp>

#endif  // auto-linking disabled

#ifdef BOOST_TEST_DYN_LINK

// ************************************************************************** //
// **************                     dll_main                 ************** //
// ************************************************************************** //

namespace boost { 

int BOOST_TEST_DECL dll_main( int (*cpp_main)( int argc, char* argv[] ), int argc, char* argv[] );

}

// ************************************************************************** //
// **************        main function for tests using dll     ************** //
// ************************************************************************** //

int cpp_main( int argc, char* argv[] );  // prototype for user's cpp_main()

int BOOST_TEST_CALL_DECL
main( int argc, char* argv[] )
{
    return ::boost::dll_main( &cpp_main, argc, argv );
}

//____________________________________________________________________________//

#endif // BOOST_TEST_DYN_LINK

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.2  2006/02/07 16:15:20  rogeeff
//  BOOST_TEST_INCLUDED guard were missing
//
//  Revision 1.1  2005/12/14 05:42:08  rogeeff
//  components primary headers
//
// ***************************************************************************

#endif // BOOST_PRG_EXEC_MONITOR_HPP_071894GER
