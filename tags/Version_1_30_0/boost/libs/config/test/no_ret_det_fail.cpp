
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_NO_UNREACHABLE_RETURN_DETECTION
// This file should not compile, if it does then
// BOOST_NO_UNREACHABLE_RETURN_DETECTION need not be defined.
// see boost_no_ret_det.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_ret_det.cxx on
// Mon Dec  9 12:06:12  2002

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifdef BOOST_NO_UNREACHABLE_RETURN_DETECTION
#include "boost_no_ret_det.cxx"
#else
#error "this file should not compile"
#endif

int cpp_main( int, char *[] )
{
   return boost_no_unreachable_return_detection::test();
}  
   
