/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_PARAM_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_HPP

#include <boost/auto_function/detail/rescan.hpp>
#include <boost/auto_function/detail/split_param_id_and_param.hpp>
#include <boost/auto_function/detail/tail_arguments.hpp>

// A macro that assigns a unique value to each different parameter kind
// This macro takes the entire argument including the name
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM( arg_in_paren )                       \
BOOST_AUTO_FUNCTION_DETAIL_RESCAN                                              \
( BOOST_AUTO_FUNCTION_DETAIL_TAIL_ARGUMENTS_AFTER_RESCAN                       \
  ( BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM( arg_in_paren ) )()    \
)

#endif // BOOST_AUTO_FUNCTION_DETAIL_PARAM_HPP
