/* compile in-place support for Boost.Wave

Copyright 2008 Ulrich Eckhardt

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
// $Id$

// This file must not be included more than once. Note that this 'once' is
// not per translation unit but per binary! Never include this in a header!
#ifdef BOOST_WAVE_COMPILE_IN_PLACE_CPP_INCLUDED
#  error "this file should only be included once per binary"
#endif

#define BOOST_WAVE_COMPILE_IN_PLACE_CPP_INCLUDED


#include <boost/../libs/wave/src/instantiate_cpp_exprgrammar.cpp>
#include <boost/../libs/wave/src/instantiate_cpp_grammar.cpp>
#include <boost/../libs/wave/src/instantiate_cpp_literalgrs.cpp>
#include <boost/../libs/wave/src/instantiate_defined_grammar.cpp>
#include <boost/../libs/wave/src/instantiate_predef_macros.cpp>
#include <boost/../libs/wave/src/instantiate_re2c_lexer.cpp>
#include <boost/../libs/wave/src/instantiate_re2c_lexer_str.cpp>
#include <boost/../libs/wave/src/token_ids.cpp>
#include <boost/../libs/wave/src/wave_config_constant.cpp>
#include <boost/../libs/wave/src/cpplexer/re2clex/aq.cpp>
#include <boost/../libs/wave/src/cpplexer/re2clex/cpp_re.cpp>

/*Boost_1_35_0/libs/wave/src/cpplexer:
#include <boost/../libs/wave/src/.cpp>
re2clex

Boost_1_35_0/libs/wave/src/cpplexer/re2clex:
aq.cpp
cpp.re
cpp_re.cpp
cpp_re.inc
strict_cpp.re
strict_cpp_re.inc
*/
