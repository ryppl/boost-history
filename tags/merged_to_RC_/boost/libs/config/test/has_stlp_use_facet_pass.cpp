
//  This file was automatically generated on Sun Jul 25 11:47:49 GMTDT 2004,
//  by libs/config/tools/generate
//  Copyright John Maddock 2002-4.
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for the most recent version.

// Test file for macro BOOST_HAS_STLP_USE_FACET
// This file should compile, if it does not then
// BOOST_HAS_STLP_USE_FACET should not be defined.
// see boost_has_stlp_use_facet.ipp for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_has_stlp_use_facet.ipp on
// Sun Jul 25 11:47:49 GMTDT 2004

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifdef BOOST_HAS_STLP_USE_FACET
#include "boost_has_stlp_use_facet.ipp"
#else
namespace boost_has_stlp_use_facet = empty_boost;
#endif

int main( int, char *[] )
{
   return boost_has_stlp_use_facet::test();
}  
   
