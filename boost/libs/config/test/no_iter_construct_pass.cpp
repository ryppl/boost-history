
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
// This file should compile, if it does not then
// BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS needs to be defined.
// see boost_no_iter_construct.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_iter_construct.cxx on
// Fri Aug 17 11:23:03  2001

#include <boost/config.hpp>
#include <boost/test/cpp_main.cpp>
#include "test.hpp"

#ifndef BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#include "boost_no_iter_construct.cxx"
#else
namespace boost_no_templated_iterator_constructors = empty_boost;
#endif

int cpp_main( int, char *[] )
{
	return boost_no_templated_iterator_constructors::test();
}	
	
