// Pinhole pinhole.h file
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Some parts based on boost::regex config.hpp

#ifndef BOOST_PINHOLE_CONFIG_HPP
#define BOOST_PINHOLE_CONFIG_HPP

#include <boost/config.hpp>

/*****************************************************************************
 *
 *  Set up dll import/export options:
 *
 ****************************************************************************/

#if defined(BOOST_HAS_DECLSPEC) && (defined(BOOST_PINHOLE_DYN_LINK) || defined(BOOST_ALL_DYN_LINK)) && !defined(BOOST_PINHOLE_STATIC_LINK)
#  if defined(BOOST_PINHOLE_SOURCE)
#     define BOOST_PINHOLE_DECL __declspec(dllexport)
#     define BOOST_PINHOLE_BUILD_DLL
#  else
#     define BOOST_PINHOLE_DECL __declspec(dllimport)
#  endif
#endif

#ifndef BOOST_PINHOLE_DECL
#  define BOOST_PINHOLE_DECL
#endif

/* Not currently supported
 
#if !defined(BOOST_PINHOLE_NO_LIB) && !defined(BOOST_PINHOLE_SOURCE) && !defined(BOOST_ALL_NO_LIB) && defined(__cplusplus)
#  define BOOST_LIB_NAME boost_pinhole
#  if defined(BOOST_PINHOLE_DYN_LINK) || defined(BOOST_ALL_DYN_LINK)
#     define BOOST_DYN_LINK
#  endif
#  ifdef BOOST_PINHOLE_DIAG
#     define BOOST_LIB_DIAGNOSTIC
#  endif
#  include <boost/config/auto_link.hpp>
#endif
*/

#endif // BOOST_PINHOLE_CONFIG_HPP
