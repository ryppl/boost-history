
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
// This file should compile, if it does not then
// BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS needs to be defined.
// see boost_BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_exp_func_tem_arg.cxx on
// Mon Jul 16 11:45:27  2001

#include <boost/config.hpp>
#include <boost/test/cpp_main.cpp>
#include "test.hpp"

#ifndef BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
#include "boost_no_exp_func_tem_arg.cxx"
#else
namespace boost_no_explicit_function_template_arguments = empty_boost;
#endif

int cpp_main( int, char *[] )
{
	return boost_no_explicit_function_template_arguments::test();
}	
	
