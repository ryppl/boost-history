/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_HPP

#include <boost/auto_function/detail/param_id.hpp>
#include <boost/preprocessor/cat.hpp>

// A macro that assigns a unique value to each different parameter kind
// This macro takes the entire argument including the name
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE( arg_in_paren )              \
BOOST_PP_CAT                                                                   \
( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_                                   \
, BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID( arg_in_paren )                          \
)

#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_auto_explicit     0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_auto_if           1
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_auto_not          2
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_auto_try          3
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_do                4
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_explicit          5
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_if                6
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_not               7
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_return            8
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_return_ref        9
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_return_rvalue_ref 10
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_return_value      11
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_try               12
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_virtual_explicit  13
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_void              14

#endif // BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_VALUE_HPP
