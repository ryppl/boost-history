/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PARENTHESES_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PARENTHESES_HPP

#include <boost/auto_function/detail/param_id.hpp>
#include <boost/preprocessor/cat.hpp>

// A macro that assigns a unique value to each different parameter kind
// This macro takes the entire argument including the name
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PARENTHESES( arg_in_paren )\
BOOST_PP_CAT                                                                   \
( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_                           \
, BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID( arg_in_paren )                          \
)

#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_auto_explicit     0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_auto_if           0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_auto_not          0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_auto_try          1
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_do                0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_explicit          0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_if                0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_not               0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_return            0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_return_ref        0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_return_rvalue_ref 0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_return_value      0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_try               1
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_virtual_explicit  0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PAREN_void              0

#endif // BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PARENTHESES_HPP
