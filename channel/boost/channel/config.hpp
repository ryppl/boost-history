/*
 *
 * Copyright (c) 1998-2002
 * John Maddock
 *
 * Copyright (c) 2003-2004
 * Douglas Gregor
 *
 * Copyright (c) 2005-2009
 * Yigong Liu
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#ifndef BOOST_CHANNEL_CONFIG_HPP
#define BOOST_CHANNEL_CONFIG_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_DECLSPEC
#  if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_CHANNEL_DYN_LINK)
#    ifdef BOOST_CHANNEL_SOURCE
#      define BOOST_CHANNEL_DECL __declspec(dllexport)
#    else
#      define BOOST_CHANNEL_DECL __declspec(dllimport)
#    endif  // BOOST_CHANNEL_SOURCE
#  endif  // DYN_LINK
#endif  // BOOST_HAS_DECLSPEC
#ifndef BOOST_CHANNEL_DECL
#  define BOOST_CHANNEL_DECL
#endif

// Setup autolinking
#if !defined(BOOST_CHANNEL_SOURCE) && !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_CHANNEL_NO_LIB)
#  define BOOST_LIB_NAME boost_channel

#  if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_CHANNEL_DYN_LINK)
#    define BOOST_DYN_LINK
#  endif

#  include <boost/config/auto_link.hpp>
#endif // autolinking on
#endif // BOOST_CHANNEL_CONFIG_HPP

