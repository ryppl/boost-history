
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_HAS_PARTIAL_STD_ALLOCATOR
// This file should compile, if it does not then
// BOOST_HAS_PARTIAL_STD_ALLOCATOR should not be defined.
// see boost_has_part_alloc.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_has_part_alloc.cxx on
// Tue Aug 13 12:19:31  2002

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include <boost/test/cpp_main.cpp>
#include "test.hpp"

#ifdef BOOST_HAS_PARTIAL_STD_ALLOCATOR
#include "boost_has_part_alloc.cxx"
#else
namespace boost_has_partial_std_allocator = empty_boost;
#endif

int cpp_main( int, char *[] )
{
   return boost_has_partial_std_allocator::test();
}  
   
