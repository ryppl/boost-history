
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_NO_CWCTYPE
// This file should compile, if it does not then
// BOOST_NO_CWCTYPE needs to be defined.
// see boost_no_cwctype.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_cwctype.cxx on
// Fri Aug 17 11:23:03  2001

#include <boost/config.hpp>
#include <boost/test/cpp_main.cpp>
#include "test.hpp"

#ifndef BOOST_NO_CWCTYPE
#include "boost_no_cwctype.cxx"
#else
namespace boost_no_cwctype = empty_boost;
#endif

int cpp_main( int, char *[] )
{
	return boost_no_cwctype::test();
}	
	
