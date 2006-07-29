//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_INDEX_HPP_20060613
#define BOOST_COROUTINE_DETAIL_INDEX_HPP_20060613
#include <boost/preprocessor/repetition.hpp>
#include <boost/coroutine/detail/arg_max.hpp>
namespace boost { namespace coroutines { namespace detail {
  /* 
   * Workaround for BOOST_PP_ENUM_BINARY_PARAMS, where a token
   * can't be createed by appending a number to a '<'.
   * NOTE: we are really abusing BOOST_PP_ENUM_BINARY_PARAMS,
   * Should simply use BOOST_PP_ENUM.
   */
  enum {BOOST_PP_ENUM_PARAMS(BOOST_COROUTINE_ARG_MAX, index_)};
} } }
#endif 
