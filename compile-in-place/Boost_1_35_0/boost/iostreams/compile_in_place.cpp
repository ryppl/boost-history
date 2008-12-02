/* compile in-place support for Boost.IOStreams

Copyright 2008 Ulrich Eckhardt

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
// $Id$

// The code must be set up for in-place compilation
#if !defined(BOOST_ALL_COMPILE_IN_PLACE) && !defined(BOOST_IOSTREAMS_COMPILE_IN_PLACE)
#  error "this file should only be included when compiling Boost.IOStreams in place"
#endif

// This file must not be included more than once. Note that this 'once' is
// not per translation unit but per binary! Never include this in a header!
#ifdef BOOST_IOSTREAMS_COMPILE_IN_PLACE_CPP_INCLUDED
#  error "this file should only be included once per binary"
#endif

#define BOOST_IOSTREAMS_COMPILE_IN_PLACE_CPP_INCLUDED

#include "../../libs/iostreams/src/bzip2.cpp"
#include "../../libs/iostreams/src/file_descriptor.cpp"
#include "../../libs/iostreams/src/mapped_file.cpp"
#include "../../libs/iostreams/src/zlib.cpp"
