/*
Copyright Redshift Software, Inc. 2008-2010
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_PREDEF_COMPILER_H
#define BOOST_PREDEF_COMPILER_H

#include <boost/predef/version_number.h>
#include <boost/predef/detail/vrp.h>

#define BOOST_OS_MACOS BOOST_VERSION_NUMBER(0,0,0)

#if defined(macintosh) || defined(Macintosh) || \
    (defined(__APPLE__) && defined(__MACH__))
    #undef BOOST_OS_MACOS
    #if defined(__APPLE__) && defined(__MACH__)
        #define BOOST_OS_MACOS BOOST_VERSION_NUMBER(10,0,0)
    #else
        #define BOOST_OS_MACOS BOOST_VERSION_NUMBER(9,0,0)
    #endif
#endif

#endif
