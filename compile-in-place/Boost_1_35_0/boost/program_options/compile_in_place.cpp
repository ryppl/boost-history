/* compile in-place support for Boost.Program_options

Copyright 2008 Ulrich Eckhardt

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
// $Id$

// The code must be set up for in-place compilation
#if !defined(BOOST_ALL_COMPILE_IN_PLACE) && !defined(BOOST_PROGRAM_OPTIONS_COMPILE_IN_PLACE)
#  error "this file should only be included when compiling Boost.Program_options in place"
#endif

// This file must not be included more than once. Note that this 'once' is
// not per translation unit but per binary! Never include this in a header!
#ifdef BOOST_PROGRAM_OPTIONS_COMPILE_IN_PLACE_CPP_INCLUDED
#  error "this file should only be included once per binary"
#endif

#define BOOST_PROGRAM_OPTIONS_COMPILE_IN_PLACE_CPP_INCLUDED

#include "../../libs/program_options/src/cmdline.cpp"
#include "../../libs/program_options/src/config_file.cpp"
#include "../../libs/program_options/src/convert.cpp"
#include "../../libs/program_options/src/options_description.cpp"
#include "../../libs/program_options/src/parsers.cpp"
#include "../../libs/program_options/src/positional_options.cpp"
#include "../../libs/program_options/src/utf8_codecvt_facet.cpp"
#include "../../libs/program_options/src/value_semantic.cpp"
#include "../../libs/program_options/src/variables_map.cpp"
#include "../../libs/program_options/src/winmain.cpp"
