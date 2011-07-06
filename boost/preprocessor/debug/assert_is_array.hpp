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
# ifndef BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_ARRAY_HPP
# define BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_ARRAY_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/comparison/equal.hpp>
# include <boost/preprocessor/control/iif.hpp>
# include <boost/preprocessor/debug/assert.hpp>
# include <boost/preprocessor/debug/assert_is_tuple.hpp>
# include <boost/preprocessor/logical/bitand.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
# include <boost/preprocessor/tuple/size.hpp>
#
# /* BOOST_PP_ASSERT_IS_ARRAY */
#
# if BOOST_PP_VARIADICS
# if defined(NDEBUG)
# define BOOST_PP_ASSERT_IS_ARRAY(x) \
/**/
# else

/*

  Check if the array is a two element tuple

*/

# define BOOST_PP_ASSERT_IS_ARRAY(x) \
    BOOST_PP_ASSERT_IS_TUPLE(x) \
    BOOST_PP_ASSERT \
      ( \
      BOOST_PP_IIF \
        ( \
        BOOST_PP_EQUAL(2,BOOST_PP_TUPLE_SIZE(x)), \
        BOOST_PP_IS_ARRAY_DETAIL_CHECK_ARRAY_FORM, \
        BOOST_PP_IS_ARRAY_DETAIL_GEN_ZERO \
        ) \
      (x) \
      ) \
/**/
#

/*

  Check if the first element of the tuple is numeric

*/

# define BOOST_PP_IS_ARRAY_DETAIL_CHECK_ARRAY_FORM(x) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_ARRAY_DETAIL_NUM(BOOST_PP_TUPLE_ELEM(0,x)), \
      BOOST_PP_IS_ARRAY_DETAIL_CHECK_NUMERIC_MATCH, \
      BOOST_PP_IS_ARRAY_DETAIL_GEN_ZERO \
      ) \
    (x) \
/**/
#

/*

  Finally check if the first element of the tuple is equal to the number of tuple elements of the second element

*/

# define BOOST_PP_IS_ARRAY_DETAIL_CHECK_NUMERIC_MATCH(x) \
    BOOST_PP_ASSERT_IS_TUPLE(BOOST_PP_TUPLE_ELEM(1,x)) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(0,x),BOOST_PP_TUPLE_SIZE(BOOST_PP_TUPLE_ELEM(1,x))), \
      1, \
      0 \
      ) \
/**/
#
# define BOOST_PP_IS_ARRAY_DETAIL_GEN_ZERO(x) \
    0 \
/**/
#
# define BOOST_PP_IS_ARRAY_DETAIL_NUM(x) BOOST_PP_IS_EMPTY(BOOST_PP_CAT(BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_, x))
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_1
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_2
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_3
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_4
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_5
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_6
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_7
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_8
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_9
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_10
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_11
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_12
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_13
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_14
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_15
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_16
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_17
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_18
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_19
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_20
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_21
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_22
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_23
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_24
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_25
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_26
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_27
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_28
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_29
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_30
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_31
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_32
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_33
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_34
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_35
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_36
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_37
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_38
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_39
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_40
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_41
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_42
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_43
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_44
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_45
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_46
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_47
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_48
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_49
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_50
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_51
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_52
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_53
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_54
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_55
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_56
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_57
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_58
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_59
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_60
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_61
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_62
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_63
# define BOOST_PP_IS_ARRAY_DETAIL_NUM_HELPER_64
#
# endif /* NDEBUG */
# endif /* BOOST_PP_VARIADICS */
# endif /* BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_ARRAY_HPP */
