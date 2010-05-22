/*
Copyright Redshift Software, Inc. 2008-2010
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_PREDEF_LIBRARY_STD_STDCPP3_H
#define BOOST_PREDEF_LIBRARY_STD_STDCPP3_H

#include <boost/predef/version_number.h>
#include <boost/predef/detail/vrp.h>

#define BOOST_LIBSTD_GNU BOOST_VERSION_NUMBER(0,0,0)

#if defined(__GLIBCPP__) || defined(__GLIBCXX__)
    #undef BOOST_LIBSTD_GNU
    #if defined(__GLIBCXX__)
        #define BOOST_LIBSTD_GNU BOOST_PREDEF_VRP_NN_NN_NN(__GLIBCXX__)
    #else
        #define BOOST_LIBSTD_GNU BOOST_PREDEF_VRP_NN_NN_NN(__GLIBCPP__)
    #endif
#endif

#endif
