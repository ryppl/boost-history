
//  This file was automatically generated on Sun Jul 25 11:47:49 GMTDT 2004,
//  by libs/config/tools/generate
//  Copyright John Maddock 2002-4.
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for the most recent version.

// Test file for macro BOOST_NO_SWPRINTF
// This file should not compile, if it does then
// BOOST_NO_SWPRINTF need not be defined.
// see boost_no_swprintf.ipp for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_swprintf.ipp on
// Sun Jul 25 11:47:49 GMTDT 2004

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifdef BOOST_NO_SWPRINTF
#include "boost_no_swprintf.ipp"
#else
#error "this file should not compile"
#endif

int main( int, char *[] )
{
   return boost_no_swprintf::test();
}  
   
