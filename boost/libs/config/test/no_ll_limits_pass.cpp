
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_NO_LONG_LONG_NUMERIC_LIMITS
// This file should compile, if it does not then
// BOOST_NO_LONG_LONG_NUMERIC_LIMITS needs to be defined.
// see boost_no_ll_limits.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_ll_limits.cxx on
// Thu Jul 18 11:26:48  2002

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include <boost/test/cpp_main.cpp>
#include "test.hpp"

#ifndef BOOST_NO_LONG_LONG_NUMERIC_LIMITS
#include "boost_no_ll_limits.cxx"
#else
namespace boost_no_long_long_numeric_limits = empty_boost;
#endif

int cpp_main( int, char *[] )
{
   return boost_no_long_long_numeric_limits::test();
}  
   
