//  (C) Copyright John Maddock 2001 - 2002. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  SGI C++ compiler setup:

#define BOOST_CXX_SGI BOOST_VERSION_NUMBER(\
    _COMPILER_VERSION/100,\
    (_COMPILER_VERSION-_COMPILER_VERSION/100*100)/10,\
    _COMPILER_VERSION-_COMPILER_VERSION/10*10)

#define BOOST_COMPILER "SGI Irix compiler version " BOOST_STRINGIZE(_COMPILER_VERSION)

#include "boost/config/compiler/common_edg.hpp"

//
// Threading support:
// Turn this on unconditionally here, it will get turned off again later
// if no threading API is detected.
//
#define BOOST_HAS_THREADS
//
// version check:
// probably nothing to do here?
