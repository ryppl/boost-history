//-----------------------------------------------------------------------------
// boost mpl/aux_/preprocessor.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_AUX_PREPROCESSOR_HPP
#define BOOST_AUX_PREPROCESSOR_HPP

#include "boost/preprocessor/arithmetic/add.hpp"
#include "boost/preprocessor/arithmetic/sub.hpp"
#include "boost/preprocessor/comma_if.hpp"
#include "boost/preprocessor/repeat.hpp"
#include "boost/preprocessor/tuple.hpp"
#include "boost/preprocessor/cat.hpp"

// BOOST_MPL_TEMPLATE_PARAMETERS(0, 0, T): <nothing>
// BOOST_MPL_TEMPLATE_PARAMETERS(0, 1, T): T0
// BOOST_MPL_TEMPLATE_PARAMETERS(0, 2, T): T0, T1
// BOOST_MPL_TEMPLATE_PARAMETERS(1, n, T): T1, T2, .., Tn

#define BOOST_MPL_AUX_TEMPLATE_PARAMETER(i, op) \
    BOOST_PREPROCESSOR_COMMA_IF(i) \
    BOOST_PREPROCESSOR_CAT( \
          BOOST_PREPROCESSOR_TUPLE_ELEM(2, 1, op) \
        , BOOST_PREPROCESSOR_ADD(BOOST_PREPROCESSOR_TUPLE_ELEM(2, 0, op), i) \
        ) \
/**/

#define BOOST_MPL_TEMPLATE_PARAMETERS(first, last, param) \
    BOOST_PREPROCESSOR_REPEAT( \
          BOOST_PREPROCESSOR_SUB(last, first) \
        , BOOST_MPL_AUX_TEMPLATE_PARAMETER \
        , (first, param) \
        ) \
/**/

// BOOST_MPL_DEFAULT_TEMPLATE_PARAMETERS(0, 0, T, int): <nothing>
// BOOST_MPL_DEFAULT_TEMPLATE_PARAMETERS(0, 1, T, int): T0 = int
// BOOST_MPL_DEFAULT_TEMPLATE_PARAMETERS(0, 2, T, int): T0 = int, T1 = int
// BOOST_MPL_DEFAULT_TEMPLATE_PARAMETERS(1, n, T, int): T1 = int, T2 = int, .., Tn = int

#define BOOST_MPL_AUX_DEFAULT_TEMPLATE_PARAMETER(i, opv) \
    BOOST_PREPROCESSOR_COMMA_IF(i) \
    BOOST_PREPROCESSOR_CAT( \
          BOOST_PREPROCESSOR_TUPLE_ELEM(3, 1, opv) \
        , BOOST_PREPROCESSOR_ADD(BOOST_PREPROCESSOR_TUPLE_ELEM(3, 0, opv), i) \
        ) = BOOST_PREPROCESSOR_TUPLE_ELEM(3, 2, opv) \
/**/

#define BOOST_MPL_DEFAULT_TEMPLATE_PARAMETERS(first, last, param, value) \
    BOOST_PREPROCESSOR_REPEAT( \
          BOOST_PREPROCESSOR_SUB(last, first) \
        , BOOST_MPL_AUX_DEFAULT_TEMPLATE_PARAMETER \
        , (first, param, value) \
        ) \
/**/

// should be in PREPROCESSOR library 
#define BOOST_MPL_PREPROCESSOR_PLUS_TOKEN() +
#define BOOST_MPL_PREPROCESSOR_MINUS_TOKEN() -

#endif // BOOST_AUX_PREPROCESSOR_HPP
