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
# ifndef BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_LIST_HPP
# define BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_LIST_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/comparison/equal.hpp>
# include <boost/preprocessor/control/iif.hpp>
# include <boost/preprocessor/control/while.hpp>
# include <boost/preprocessor/debug/assert.hpp>
# include <boost/preprocessor/debug/assert_is_tuple.hpp>
# include <boost/preprocessor/facilities/is_empty.hpp>
# include <boost/preprocessor/facilities/is_tuple_begin.hpp>
# include <boost/preprocessor/logical/not.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
# include <boost/preprocessor/tuple/size.hpp>
#
# /* BOOST_PP_ASSERT_IS_LIST */
#
# if BOOST_PP_VARIADICS
# if defined(NDEBUG)
# define BOOST_PP_ASSERT_IS_LIST(x) \
/**/
#
# else
#
# define BOOST_PP_ASSERT_IS_LIST(x) \
    BOOST_PP_IS_LIST_DETAIL_GEN_EMPTY \
      ( \
      BOOST_PP_WHILE \
        ( \
        BOOST_PP_IS_LIST_DETAIL_PRED, \
        BOOST_PP_IS_LIST_DETAIL_OP, \
        x \
        ) \
      ) \
/**/
#
# define BOOST_PP_IS_LIST_DETAIL_GEN_EMPTY(x) \
    BOOST_PP_IS_LIST_DETAIL_GEN_EMPTY_I(x) \
/**/
#
# define BOOST_PP_IS_LIST_DETAIL_GEN_EMPTY_I(x)
#
#define BOOST_PP_IS_LIST_DETAIL_PRED(d,state) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_TUPLE_BEGIN(state), \
      BOOST_PP_IS_LIST_DETAIL_GEN_ONE, \
      BOOST_PP_IS_LIST_DETAIL_NOT_BOOST_PP_NIL \
      ) \
    (state) \
/**/
#
#define BOOST_PP_IS_LIST_DETAIL_OP(d,state) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_TUPLE_BEGIN(state), \
      BOOST_PP_IS_LIST_DETAIL_PROCESS_TWO_ELEMENT_TUPLE, \
      BOOST_PP_IS_LIST_DETAIL_PROCESS_IF_BOOST_PP_NIL \
      ) \
    (state) \
/**/
#
# define BOOST_PP_IS_LIST_DETAIL_PROCESS_TWO_ELEMENT_TUPLE(x) \
    BOOST_PP_ASSERT_IS_TUPLE(x) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_EQUAL(2,BOOST_PP_TUPLE_SIZE(x)), \
      BOOST_PP_IS_LIST_DETAIL_RETURN_SECOND, \
      BOOST_PP_IS_LIST_DETAIL_ASSERT \
      ) \
    (x) \
/**/
#
# define BOOST_PP_IS_LIST_DETAIL_RETURN_SECOND(x) \
    BOOST_PP_TUPLE_ELEM(1,x) \
/**/
#
#define BOOST_PP_IS_LIST_DETAIL_PROCESS_IF_BOOST_PP_NIL(x) \
    BOOST_PP_ASSERT \
      ( \
      BOOST_PP_IS_LIST_DETAIL_BOOST_PP_NIL(x) \
      ) \
    BOOST_PP_NIL \
/**/
#
# define BOOST_PP_IS_LIST_DETAIL_ASSERT(x) \
    BOOST_PP_ASSERT(0) \
/**/
#
# define BOOST_PP_IS_LIST_DETAIL_GEN_ONE(x) \
    1 \
/**/
#
# define BOOST_PP_IS_LIST_DETAIL_NOT_BOOST_PP_NIL(x) \
    BOOST_PP_NOT \
      ( \
      BOOST_PP_IS_LIST_DETAIL_BOOST_PP_NIL(x) \
      ) \
/**/
#
# define BOOST_PP_IS_LIST_DETAIL_BOOST_PP_NIL(x) \
    BOOST_PP_IS_EMPTY \
      ( \
      BOOST_PP_CAT \
        ( \
        BOOST_PP_IS_LIST_DETAIL_NIL_HELPER_, \
        x \
        ) \
      ) \
/**/
#
# define BOOST_PP_IS_LIST_DETAIL_NIL_HELPER_BOOST_PP_NIL
#
# endif /* NDEBUG */
# endif /* BOOST_PP_VARIADICS */
# endif /* BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_LIST_HPP */
