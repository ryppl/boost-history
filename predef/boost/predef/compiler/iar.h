/*
Copyright Redshift Software, Inc. 2008-2010
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_PREDEF_COMPILER_IAR_H
#define BOOST_PREDEF_COMPILER_IAR_H

#include <boost/predef/version_number.h>
#include <boost/predef/detail/vrp.h>

#define BOOST_CXX_IAR BOOST_VERSION_NUMBER(0,0,0)

#if defined(__IAR_SYSTEMS_ICC__)
    #undef BOOST_CXX_IAR
    #define BOOST_CXX_IAR BOOST_PREDEF_VRP_NN_NN(__VER__)
#endif

#endif
