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
#if !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_TEST_NO_LIB) && \
    !defined(BOOST_TEST_SOURCE) && !defined(BOOST_TEST_INCLUDED)

#  define BOOST_LIB_NAME boost_test_exec_monitor
#  include <boost/config/auto_link.hpp>

#endif  // auto-linking disabled

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.3  2006/03/19 11:45:26  rogeeff
//  main function renamed for consistancy
//
//  Revision 1.2  2006/02/07 16:15:20  rogeeff
//  BOOST_TEST_INCLUDED guard were missing
//
//  Revision 1.1  2005/12/14 05:42:08  rogeeff
//  components primary headers
//
// ***************************************************************************

#endif // BOOST_TEST_EXEC_MONITOR_HPP_071894GER
