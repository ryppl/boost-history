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
# ifndef BOOST_PREPROCESSOR_TUPLE_USE_OVERLOAD_HPP
# define BOOST_PREPROCESSOR_TUPLE_USE_OVERLOAD_HPP
#
# include <boost/preprocessor/config/variadics.hpp>
#
# if BOOST_PP_VARIADICS
#
# include <boost/preprocessor/facilities/overload.hpp>
#
# /* BOOST_PP_TUPLE_DETAIL_USE_OVERLOAD */
#
#    define BOOST_PP_TUPLE_DETAIL_USE_OVERLOAD(prefix,...) \
         BOOST_PP_TUPLE_DETAIL_APPLY_VAR(BOOST_PP_OVERLOAD(prefix, __VA_ARGS__),(__VA_ARGS__)) \
         /**/
#
#define BOOST_PP_TUPLE_DETAIL_APPLY_VAR_I(macro, args) \
  macro args \
/**/
#define BOOST_PP_TUPLE_DETAIL_APPLY_VAR(macro, args) \
  BOOST_PP_TUPLE_DETAIL_APPLY_VAR_I(macro, args) \
/**/
# endif // BOOST_PP_VARIADICS
# endif // BOOST_PREPROCESSOR_TUPLE_USE_OVERLOAD_HPP
