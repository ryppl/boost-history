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
# ifndef BOOST_PREPROCESSOR_TUPLE_TO_ARRAY_HPP
# define BOOST_PREPROCESSOR_TUPLE_TO_ARRAY_HPP
#
# include <boost/preprocessor/config/variadics.hpp>
#
#if BOOST_PP_VARIADICS
#
# include <boost/preprocessor/facilities/overload.hpp>
# include <boost/preprocessor/tuple/size.hpp>
#
# /* BOOST_PP_TUPLE_TO_ARRAY */
#
# define BOOST_PP_TUPLE_TO_ARRAY(...) \
  BOOST_PP_OVERLOAD(BOOST_PP_TUPLE_DETAIL_TO_ARRAY_, __VA_ARGS__)(__VA_ARGS__) \
  /**/
# define BOOST_PP_TUPLE_DETAIL_TO_ARRAY_1(tuple) \
  BOOST_PP_TUPLE_DETAIL_TO_ARRAY_2(BOOST_PP_TUPLE_SIZE(tuple),tuple) \
  /**/
# define BOOST_PP_TUPLE_DETAIL_TO_ARRAY_2(size, tuple) \
  ( size, tuple ) \
/**/
#
# else
#
# /* BOOST_PP_TUPLE_TO_ARRAY */
#
#define BOOST_PP_TUPLE_TO_ARRAY(size,tuple) \
  ( size, tuple ) \
/**/
#
# endif // BOOST_PP_VARIADICS
# endif // BOOST_PREPROCESSOR_TUPLE_TO_ARRAY_HPP
