/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE_HPP

#include <boost/auto_function/detail/param_id_category.hpp>
#include <boost/preprocessor/cat.hpp>

// Whenever a category value is added, the category macros must also be updated
// in <boost/auto_function/detail/param_id_category.hpp>

#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE( arg_in_paren )     \
BOOST_PP_CAT( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE_              \
            , BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY( arg_in_paren )     \
            )

// Macros that associate each param category ID with a unique value
// Note: If a category is added, it is important to update
// BOOST_AUTO_FUNCTION_DETAIL_NUM_PARAM_CATEGORIES as well
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE_auto_if           0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE_auto_try          1
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE_do                2
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE_explicit          3
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE_if                4
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE_try               5

// Update this whenever a new category is added
#define BOOST_AUTO_FUNCTION_DETAIL_NUM_PARAM_CATEGORIES 6

#endif // BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE_HPP
