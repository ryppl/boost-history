
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for the most recent version.

// Test file for macro BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
// This file should not compile, if it does then
// BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION need not be defined.
// see boost_no_partial_spec.ipp for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_partial_spec.ipp on
// Tue Jun 15 11:47:07 GMTST 2004

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include "boost_no_partial_spec.ipp"
#else
#error "this file should not compile"
#endif

int main( int, char *[] )
{
   return boost_no_template_partial_specialization::test();
}  
   
