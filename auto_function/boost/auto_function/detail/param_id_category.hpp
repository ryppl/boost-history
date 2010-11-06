/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_HPP

#include <boost/auto_function/detail/param_id.hpp>
#include <boost/preprocessor/cat.hpp>

// Whenever a category is added, the value macros must also be updated in
// <boost/auto_function/detail/param_id_category_value.hpp>

#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY( arg_in_paren )           \
BOOST_PP_CAT( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_                    \
            , BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID( arg_in_paren )              \
            )

// Macros that associate param IDs with the category that they belong to
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_auto_if  auto_if
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_auto_not auto_if

#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_auto_try auto_try

#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_auto_explicit    explicit
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_explicit         explicit
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_virtual_explicit explicit

#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_do                do
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_return            do
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_return_ref        do
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_return_rvalue_ref do
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_return_value      do
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_void              do

#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_if  if
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_not if

#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_try try

#endif // BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_HPP
