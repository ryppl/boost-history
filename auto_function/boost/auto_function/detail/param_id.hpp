/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HPP

#include <boost/auto_function/detail/head_argument.hpp>
#include <boost/auto_function/detail/split_param_id_and_param.hpp>

// A macro that assigns a single-token name to each parambased on its param id
// (it removes spaces from multiword param ids and replaces them with _)
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID( arg_in_paren )                    \
BOOST_AUTO_FUNCTION_DETAIL_HEAD_ARGUMENT_AFTER_RESCAN                          \
( BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM( arg_in_paren ) )

#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID( param_id )                       \
BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_ ## param_id

#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_auto_explicit     auto explicit
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_auto_if           auto if
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_auto_not          auto not
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_auto_try          auto try
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_do                do
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_explicit          explicit
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_if                if
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_not               not
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_return            return
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_return_ref        return_ref
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_return_rvalue_ref return_rvalue_ref
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_return_value      return_value
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_try               try
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_virtual_explicit  virtual explicit
#define BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID_void              void

#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID( param_id )                         \
BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_ ## param_id

#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_auto_explicit     AUTO_EXPLICIT
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_auto_if           AUTO_IF
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_auto_not          AUTO_NOT
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_auto_try          AUTO_TRY
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_do                DO
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_explicit          EXPLICIT
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_if                IF
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_not               NOT
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_return            RETURN
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_return_ref        RETURN_REF
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_return_rvalue_ref RETURN_RVALUE_REF
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_return_value      RETURN_VALUE
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_try               TRY
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_virtual_explicit  VIRTUAL_EXPLICIT
#define BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID_void              VOID

#endif // BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HPP
