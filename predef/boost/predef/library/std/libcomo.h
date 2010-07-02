/*
Copyright Redshift Software, Inc. 2008-2010
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_PREDEF_LIBRARY_STD_LIBCOMO_H
#define BOOST_PREDEF_LIBRARY_STD_LIBCOMO_H

#include <boost/predef/version_number.h>
#include <boost/predef/detail/vrp.h>

#define BOOST_LIBSTD_COMO BOOST_VERSION_NUMBER(0,0,0)

#if defined(__LIBCOMO__)
    #undef BOOST_LIBSTD_COMO
    #define BOOST_LIBSTD_COMO BOOST_VERSION_NUMBER(__LIBCOMO_VERSION__,0,0)
#endif

#endif