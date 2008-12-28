/*  Copyright 2008 Ulrich Eckhardt

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
// $Id$

// The code must be set up for in-place compilation
#if !defined(BOOST_ALL_COMPILE_IN_PLACE) && !defined(BOOST_THREAD_COMPILE_IN_PLACE)
#  error "this file should only be included when compiling Boost.Thread in place"
#endif

// This file must not be included more than once. Note that this 'once' is
// not per translation unit but per binary! Never include this in a header!
#ifdef BOOST_THREAD_COMPILE_IN_PLACE_CPP_INCLUDED
#  error "this file should only be included once per binary"
#endif

/* Note: this isn't a working include guard, but it's the best we can do here.
If the user includes this in more than one translation unit they will simply
get linker errors. */
#define BOOST_THREAD_COMPILE_IN_PLACE_CPP_INCLUDED


#if defined(BOOST_THREAD_PLATFORM_WIN32)
#  include "../../libs/thread/src/win32/exceptions.cpp"
#  include "../../libs/thread/src/win32/thread.cpp"
#  include "../../libs/thread/src/win32/tss_dll.cpp"
#  include "../../libs/thread/src/win32/tss_pe.cpp"
#elif defined(BOOST_THREAD_PLATFORM_PTHREAD)
#  include "../../libs/thread/src/pthread/exceptions.cpp"
#  include "../../libs/thread/src/pthread/once.cpp"
#  include "../../libs/thread/src/pthread/thread.cpp"
#else
#  error "Boost threads unavailable on this platform"
#endif

