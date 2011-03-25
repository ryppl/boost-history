# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_VARIADIC_DETAIL_TO_LIST_HPP
# define BOOST_PREPROCESSOR_VARIADIC_DETAIL_TO_LIST_HPP
#
# include <boost/preprocessor/config/variadics.hpp>
#
#if BOOST_PP_VARIADICS
#
#include <boost/preprocessor/tuple/to_list.hpp>
#include "size.hpp"
#include "to_tuple.hpp"
#
#define BOOST_PP_VARIADIC_DETAIL_TO_LIST(...) \
  BOOST_PP_VARIADIC_DETAIL_TUPLE_TO_LIST(BOOST_PP_VARIADIC_DETAIL_SIZE(__VA_ARGS__),BOOST_PP_VARIADIC_DETAIL_TO_TUPLE(__VA_ARGS__)) \
/**/
#define BOOST_PP_VARIADIC_DETAIL_TUPLE_TO_LIST(size,tuple) \
  BOOST_PP_TUPLE_TO_LIST(size,tuple) \
/**/
#endif // BOOST_PP_VARIADICS
#endif // BOOST_PREPROCESSOR_VARIADIC_DETAIL_TO_LIST_HPP
