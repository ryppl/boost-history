
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for the most recent version.

// Test file for macro BOOST_NO_FUNCTION_TEMPLATE_ORDERING
// This file should compile, if it does not then
// BOOST_NO_FUNCTION_TEMPLATE_ORDERING needs to be defined.
// see boost_no_func_tmp_order.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_func_tmp_order.cxx on
// Tue Oct  7 11:26:18 GMTST 2003

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#include "boost_no_func_tmp_order.cxx"
#else
namespace boost_no_function_template_ordering = empty_boost;
#endif

int cpp_main( int, char *[] )
{
   return boost_no_function_template_ordering::test();
}  
   
