//-----------------------------------------------------------------------------
// boost mpl/aux_/config/dtp.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_AUX_CONFIG_COMPILER_HPP_INCLUDED
#define BOOST_MPL_AUX_CONFIG_COMPILER_HPP_INCLUDED

#include "boost/mpl/aux_/config/dtp.hpp"
#include "boost/config.hpp"

#if defined(BOOST_MSVC) && BOOST_MSVC < 1300
#   define BOOST_MPL_COMPILER_CLASS msvc60
#elif defined(BOOST_MSVC) && BOOST_MSVC == 1300
#   define BOOST_MPL_COMPILER_CLASS msvc70
#elif defined(__GNUC__)
#   define BOOST_MPL_COMPILER_CLASS gcc
#elif defined(__BORLANDC__)
#   define BOOST_MPL_COMPILER_CLASS bcc
#elif defined(__MWERKS__) && __MWERKS__ <= 0x3001
#   define BOOST_MPL_COMPILER_CLASS mwcw
#elif defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#   define BOOST_MPL_COMPILER_CLASS no_ctps
#else
#   define BOOST_MPL_COMPILER_CLASS plain
#endif

#define BOOST_MPL_COMPILER_DIR BOOST_MPL_COMPILER_CLASS/

#endif // BOOST_MPL_AUX_CONFIG_COMPILER_HPP_INCLUDED
