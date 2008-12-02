/* compile in-place support for Boost.Regex

Copyright 2008 Ulrich Eckhardt

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
// $Id$

// The code must be set up for in-place compilation
#if !defined(BOOST_ALL_COMPILE_IN_PLACE) && !defined(BOOST_REGEX_COMPILE_IN_PLACE)
#  error "this file should only be included when compiling Boost.Regex in place"
#endif

// This file must not be included more than once. Note that this 'once' is
// not per translation unit but per binary! Never include this in a header!
#ifdef BOOST_REGEX_COMPILE_IN_PLACE_CPP_INCLUDED
#  error "this file should only be included once per binary"
#endif

#define BOOST_REGEX_COMPILE_IN_PLACE_CPP_INCLUDED
/* Note: the three *instances.cpp files actually only define some macros and
then include boost/regex/v4/instances.hpp which instantiates some templates.
However, instead of really including that file, they include boost/regex.hpp
which has include guards, so it is impossible to compile these three files as
they are into the same translation unit. The result is the code below, which
is labeled "template instances". */
#include "../../libs/regex/src/c_regex_traits.cpp"
#include "../../libs/regex/src/cpp_regex_traits.cpp"
#include "../../libs/regex/src/cregex.cpp"
#include "../../libs/regex/src/fileiter.cpp"
#include "../../libs/regex/src/icu.cpp"
#include "../../libs/regex/src/instances.cpp"
#include "../../libs/regex/src/posix_api.cpp"
#include "../../libs/regex/src/regex.cpp"
#include "../../libs/regex/src/regex_debug.cpp"
#include "../../libs/regex/src/regex_raw_buffer.cpp"
#include "../../libs/regex/src/regex_traits_defaults.cpp"
#include "../../libs/regex/src/static_mutex.cpp"
#include "../../libs/regex/src/usinstances.cpp"
#include "../../libs/regex/src/w32_regex_traits.cpp"
#include "../../libs/regex/src/wc_regex_traits.cpp"
#include "../../libs/regex/src/wide_posix_api.cpp"
#include "../../libs/regex/src/winstances.cpp"


//
// Note: the code below is copied from boost/regex/v4/regex.hpp
//
// template instances:
//
#define BOOST_REGEX_CHAR_T char
#ifdef BOOST_REGEX_NARROW_INSTANTIATE
#  define BOOST_REGEX_INSTANTIATE
#endif
#include <boost/regex/v4/instances.hpp>
#undef BOOST_REGEX_CHAR_T
#ifdef BOOST_REGEX_INSTANTIATE
#  undef BOOST_REGEX_INSTANTIATE
#endif

#ifndef BOOST_NO_WREGEX
#  define BOOST_REGEX_CHAR_T wchar_t
#  ifdef BOOST_REGEX_WIDE_INSTANTIATE
#    define BOOST_REGEX_INSTANTIATE
#  endif
#  include <boost/regex/v4/instances.hpp>
#  undef BOOST_REGEX_CHAR_T
#  ifdef BOOST_REGEX_INSTANTIATE
#    undef BOOST_REGEX_INSTANTIATE
#  endif
#endif

#if !defined(BOOST_NO_WREGEX) && defined(BOOST_REGEX_HAS_OTHER_WCHAR_T)
#  define BOOST_REGEX_CHAR_T unsigned short
#  ifdef BOOST_REGEX_US_INSTANTIATE
#    define BOOST_REGEX_INSTANTIATE
#  endif
#  include <boost/regex/v4/instances.hpp>
#  undef BOOST_REGEX_CHAR_T
#  ifdef BOOST_REGEX_INSTANTIATE
#    undef BOOST_REGEX_INSTANTIATE
#  endif
#endif

