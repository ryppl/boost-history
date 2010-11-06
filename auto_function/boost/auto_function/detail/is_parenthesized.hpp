/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_STARTS_PARENTHESIZED_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_STARTS_PARENTHESIZED_HPP

#include <boost/auto_function/detail/128_args.hpp>
#include <boost/auto_function/detail/num_args.hpp>
#include <boost/preprocessor/comparison/equal.hpp>

#define BOOST_AUTO_FUNCTION_DETAIL_STARTS_PARENTHESIZED( arg )                 \
BOOST_PP_EQUAL                                                                 \
( BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS                                          \
  ( BOOST_AUTO_FUNCTION_DETAIL_STARTS_PARENTHESIZED_IMPL arg )                 \
, 2                                                                            \
)

#define BOOST_AUTO_FUNCTION_DETAIL_STARTS_PARENTHESIZED_IMPL( ... ) _1,_2

#endif // BOOST_AUTO_FUNCTION_DETAIL_STARTS_PARENTHESIZED_HPP
