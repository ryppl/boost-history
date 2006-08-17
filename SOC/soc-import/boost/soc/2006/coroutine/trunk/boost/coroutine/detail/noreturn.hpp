//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_NORETURN_HPP_20060812
#define BOOST_COROUTINE_DETAIL_NORETURN_HPP_20060812
/*
 * The BOOST_COROUTINE_NORETURN macro provides a way to 
 * tell the compiler that a function will not return through
 * the normal return path (it could return throgh a thrown exception).
 * This not only provieds a possible optimization hint, but also
 * prevents the compiler from complaining if a function that call
 * a noreturn function does not call return itself.
 */
#include <boost/config.hpp>

#if defined(__GNUC__)

#define BOOST_COROUTINE_NORETURN(function) \
    function __attribute__((__noreturn__)) \
/**/
  
#elif defined (BOOST_MSVC)

#define BOOST_COROUTINE_NORETURN(function) \
    __declspec(noreturn) function          \
/**/

#else
//just for testing, remove the following error.
#error no default
#define BOOST_COROUTINE_NORETURN(function) \
    function
/**/

#endif

#endif
