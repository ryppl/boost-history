
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_NO_STD_WSTRING
// This file should compile, if it does not then
// BOOST_NO_STD_WSTRING needs to be defined.
// see boost_no_std_wstring.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_std_wstring.cxx on
// Mon Sep 10 12:18:12  2001

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include <boost/test/cpp_main.cpp>
#include "test.hpp"

#ifndef BOOST_NO_STD_WSTRING
#include "boost_no_std_wstring.cxx"
#else
namespace boost_no_std_wstring = empty_boost;
#endif

int cpp_main( int, char *[] )
{
   return boost_no_std_wstring::test();
}  
   
