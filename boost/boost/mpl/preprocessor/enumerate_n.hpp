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

#include "boost/preprocessor/arithmetic/add.hpp"
#include "boost/preprocessor/arithmetic/sub.hpp"
#include "boost/preprocessor/repeat.hpp"
#include "boost/preprocessor/comma_if.hpp"
#include "boost/preprocessor/cat.hpp"

#define BOOST_MPL_NUMBERED_EXPRESSION(i, expr_n_tp)                           \
    BOOST_PREPROCESSOR_COMMA_IF(i)                                            \
    BOOST_PREPROCESSOR_CAT(                                                   \
        BOOST_PREPROCESSOR_TUPLE_ELEM(2, 0, expr_n_tp)                        \
      , BOOST_PREPROCESSOR_ADD(                                               \
            i                                                                 \
          , BOOST_PREPROCESSOR_TUPLE_ELEM(2, 1, expr_n_tp)                    \
          )                                                                   \
      )                                                                       \
/**/

#define BOOST_MPL_ENUMERATE_N(n, expr)                                        \
    BOOST_PREPROCESSOR_REPEAT(                                                \
        n                                                                     \
      , BOOST_MPL_NUMBERED_EXPRESSION                                         \
      , (expr, 0)                                                             \
      )                                                                       \
/**/

#define BOOST_MPL_ENUMERATE_FROM_N_TO_M(n, m, expr)                           \
    BOOST_PREPROCESSOR_REPEAT(                                                \
        BOOST_PREPROCESSOR_SUB(m, n)                                          \
      , BOOST_MPL_NUMBERED_EXPRESSION                                         \
      , (expr, n)                                                             \
      )                                                                       \
/**/

#endif // BOOST_MPL_PREPROCESSOR_ENUMERATE_N_HPP
