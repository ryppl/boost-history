
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for the most recent version.

// Test file for macro BOOST_MSVC6_MEMBER_TEMPLATES
// This file should not compile, if it does then
// BOOST_MSVC6_MEMBER_TEMPLATES may be defined.
// see boost_has_vc6_mem_templ.ipp for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_has_vc6_mem_templ.ipp on
// Tue Jun 15 11:47:07 GMTST 2004

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifndef BOOST_MSVC6_MEMBER_TEMPLATES
#include "boost_has_vc6_mem_templ.ipp"
#else
#error "this file should not compile"
#endif

int main( int, char *[] )
{
   return boost_msvc6_member_templates::test();
}  
   
