//-----------------------------------------------------------------------------
// boost mpl/preprocessor/repeat_n.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_PREPROCESSOR_REPEAT_N_HPP
#define BOOST_MPL_PREPROCESSOR_REPEAT_N_HPP

#include "boost/preprocessor/repeat.hpp"

#define BOOST_MPL_IDENTITY_MACRO(n, expr) expr
#define BOOST_MPL_REPEAT_N(n, expr)                                           \
    BOOST_PREPROCESSOR_REPEAT(                                                \
        n                                                                     \
      , BOOST_MPL_IDENTITY_MACRO                                              \
      , expr                                                                  \
      )                                                                       \
/**/

#endif // BOOST_MPL_PREPROCESSOR_REPEAT_N_HPP
