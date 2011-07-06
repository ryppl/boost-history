# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_TUPLE_HPP
# define BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_TUPLE_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/control/iif.hpp>
# include <boost/preprocessor/debug/assert.hpp>
# include <boost/preprocessor/facilities/is_empty.hpp>
# include <boost/preprocessor/facilities/is_tuple_begin.hpp>
#
# /* BOOST_PP_ASSERT_IS_TUPLE */
#
# if BOOST_PP_VARIADICS
# if defined(NDEBUG)
# define BOOST_PP_ASSERT_IS_TUPLE(x) \
/**/
# else
# define BOOST_PP_ASSERT_IS_TUPLE(x) \
    BOOST_PP_ASSERT \
      ( \
      BOOST_PP_IIF \
        ( \
        BOOST_PP_IS_TUPLE_BEGIN(x), \
        BOOST_PP_IS_TUPLE_DETAIL_IS_NOT_AFTER, \
        BOOST_PP_IS_TUPLE_DETAIL_GEN_ZERO \
        ) \
      (x) \
      ) \
/**/
#
# define BOOST_PP_IS_TUPLE_DETAIL_IS_NOT_AFTER(x) \
    BOOST_PP_IS_EMPTY(BOOST_PP_IS_TUPLE_DETAIL_EXPAND_AFTER x) \
/**/
#
# define BOOST_PP_IS_TUPLE_DETAIL_EXPAND_AFTER(...) \
/**/
#
# define BOOST_PP_IS_TUPLE_DETAIL_GEN_ZERO(x) \
    0 \
/**/
#
# endif /* NDEBUG */
# endif /* BOOST_PP_VARIADICS */
# endif /* BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_TUPLE_HPP */
