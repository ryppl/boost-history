/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG_HPP

#include <boost/auto_function/detail/param_id_category.hpp>
#include <boost/preprocessor/cat.hpp>

// A macro that assigns a unique value to each different parameter kind
// This macro takes the entire argument including the name
#define BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG( arg_in_paren )          \
BOOST_PP_CAT                                                                   \
( BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG_                               \
, BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY( arg_in_paren )                 \
)

#define BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG_auto_if           1
#define BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG_auto_try          1
#define BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG_do                0
#define BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG_explicit          0
#define BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG_if                1
#define BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG_try               1

#endif // BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG_HPP
