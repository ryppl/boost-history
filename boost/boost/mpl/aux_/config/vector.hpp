
#ifndef BOOST_MPL_AUX_CONFIG_VECTOR_HPP_INCLUDED
#define BOOST_MPL_AUX_CONFIG_VECTOR_HPP_INCLUDED

// + file: boost/mpl/vector/vector0.hpp
// + last modified: 12/may/03

// Copyright (c) 2000-03
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.
//
// See http://www.boost.org/libs/mpl for documentation.

// agurt, 10/jul/02: full-fledged __typeof is needed to permit the optimal 
// vector implementation

#if !defined(BOOST_MPL_CFG_VECTOR_IMPL_DIR)

#   if defined(__MWERKS__) && __MWERKS__ >= 0x3001 \
    || defined(__GNUC__) && __GNUC__ >= 3
#       define BOOST_MPL_CFG_VECTOR_IMPL_DIR typeof_based
#   else
#       define BOOST_MPL_CFG_VECTOR_IMPL_DIR plain
#   endif

#endif

#endif // BOOST_MPL_AUX_CONFIG_VECTOR_HPP_INCLUDED
