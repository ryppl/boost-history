//-----------------------------------------------------------------------------
// boost mpl/preprocessor/enumerate_n.hpp header file
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

#ifndef BOOST_MPL_PREPROCESSOR_ENUMERATE_N_HPP
#define BOOST_MPL_PREPROCESSOR_ENUMERATE_N_HPP

#include "boost/preprocessor/add.hpp"
#include "boost/preprocessor/sub.hpp"
#include "boost/preprocessor/repeat.hpp"
#include "boost/preprocessor/comma_if.hpp"
#include "boost/preprocessor/cat.hpp"

#define BOOST_MPL_NUMBERED_EXPRESSION(i, expr, n)                             \
    BOOST_PREPROCESSOR_COMMA_IF(i)                                            \
    BOOST_PREPROCESSOR_CAT(expr, BOOST_PREPROCESSOR_ADD(i, n))                \
/**/

#define BOOST_MPL_ENUMERATE_N(n, expr)                                        \
    BOOST_PREPROCESSOR_REPEAT(                                                \
        n                                                                     \
      , BOOST_MPL_NUMBERED_EXPRESSION                                         \
      , expr                                                                  \
      , 0                                                                     \
      )                                                                       \
/**/

#define BOOST_MPL_ENUMERATE_FROM_N_TO_M(n, m, expr)                           \
    BOOST_PREPROCESSOR_REPEAT(                                                \
        BOOST_PREPROCESSOR_SUB(m, n)                                          \
      , BOOST_MPL_NUMBERED_EXPRESSION                                         \
      , expr                                                                  \
      , n                                                                     \
      )                                                                       \
/**/

#endif // BOOST_MPL_PREPROCESSOR_ENUMERATE_N_HPP
