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
# include <boost/preprocessor/debug/assert.hpp>
# include <boost/preprocessor/debug/assert_is_tuple.hpp>
# include <boost/preprocessor/facilities/is_tuple_begin.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
# include <boost/preprocessor/tuple/size.hpp>
#
# /* BOOST_PP_ASSERT_IS_LIST */
#
# if BOOST_PP_VARIADICS
# if defined(NDEBUG)
# define BOOST_PP_ASSERT_IS_LIST(x) \
/**/
# else

/*

  Check if the list starts with a tuple or is BOOST_PP_NIL

*/
# define BOOST_PP_ASSERT_IS_LIST(x) \
    BOOST_PP_ASSERT \
      ( \
      BOOST_PP_IIF \
        ( \
        BOOST_PP_IS_TUPLE_BEGIN(x), \
        BOOST_PP_IS_LIST_DETAIL_TWO_ELEMENT_TUPLE, \
        BOOST_PP_IS_LIST_DETAIL_BOOST_PP_NIL \
        ) \
      (x) \
      ) \
/**/
#

/*

  Check if we have a two element tuple

*/

# define BOOST_PP_IS_LIST_DETAIL_TWO_ELEMENT_TUPLE(x) \
    BOOST_PP_ASSERT_IS_TUPLE(x) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_EQUAL(2,BOOST_PP_TUPLE_SIZE(x)), \
      BOOST_PP_IS_LIST_DETAIL_CHECK_LIST_FORM, \
      BOOST_PP_IS_LIST_DETAIL_GEN_ZERO \
      ) \
    (x) \
/**/
#

/*

  Check if the first element does not begin as a tuple

*/

# define BOOST_PP_IS_LIST_DETAIL_CHECK_LIST_FORM(x) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_TUPLE_BEGIN(BOOST_PP_TUPLE_ELEM(0,x)), \
      BOOST_PP_IS_LIST_DETAIL_GEN_ZERO, \
      BOOST_PP_IS_LIST_DETAIL_SECOND \
      ) \
    (x) \
/**/
#

/*

  Finally check if the second element is a list

*/

# define BOOST_PP_IS_LIST_DETAIL_SECOND(x) \
    BOOST_PP_ASSERT_IS_LIST(BOOST_PP_TUPLE_ELEM(1,x)) \
    1 \
/**/
#
# define BOOST_PP_IS_LIST_DETAIL_GEN_ZERO(x) \
    0 \
/**/
#
# define BOOST_PP_IS_LIST_DETAIL_BOOST_PP_NIL(x) BOOST_PP_IS_EMPTY(BOOST_PP_CAT(BOOST_PP_IS_LIST_DETAIL_NIL_HELPER_, x))
# define BOOST_PP_IS_LIST_DETAIL_NIL_HELPER_BOOST_PP_NIL
#
# endif /* NDEBUG */
# endif /* BOOST_PP_VARIADICS */
# endif /* BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_LIST_HPP */
