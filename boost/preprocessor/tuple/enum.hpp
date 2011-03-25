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
# ifndef BOOST_PREPROCESSOR_TUPLE_ENUM_HPP
# define BOOST_PREPROCESSOR_TUPLE_ENUM_HPP
#
# include <boost/preprocessor/config/variadics.hpp>
#
#if BOOST_PP_VARIADICS
#
# include <boost/preprocessor/facilties/overload.hpp>
#
#define BOOST_PP_TUPLE_ENUM(...) \
  BOOST_PP_OVERLOAD(BOOST_PP_TUPLE_ENUM_, __VA_ARGS__)(__VA_ARGS__) \
/**/
#define BOOST_PP_TUPLE_ENUM_1(tuple) \
  BOOST_PP_TUPLE_DETAIL_ENUM(tuple) \
/**/
#define BOOST_PP_TUPLE_ENUM_2(size, tuple) \
  BOOST_PP_TUPLE_ENUM_1(tuple) \
/**/
#define BOOST_PP_TUPLE_DETAIL_REMOVE_TUPLE_PARENS(...) \
  __VA_ARGS__ \
/**/
#define BOOST_PP_TUPLE_DETAIL_ENUM(tuple) \
  BOOST_PP_TUPLE_DETAIL_REMOVE_TUPLE_PARENS tuple \
/**/
#else
#
#include <boost/preprocessor/tuple/rem.hpp>
#
#define BOOST_PP_TUPLE_ENUM(size, tuple) \
  BOOST_PP_TUPLE_REM_CTOR(size, tuple) \
/**/
#endif // BOOST_PP_VARIADICS
#endif // BOOST_PREPROCESSOR_TUPLE_ENUM_HPP
