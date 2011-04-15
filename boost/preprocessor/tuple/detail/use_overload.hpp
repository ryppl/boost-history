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
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
# include <boost/preprocessor/facilities/empty.hpp>
# endif
#
# /* BOOST_PP_TUPLE_DETAIL_ELEM_USE_OVERLOAD */
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_TUPLE_DETAIL_ELEM_USE_OV_CAT(a, b) BOOST_PP_TUPLE_DETAIL_ELEM_USE_OV_CAT_I(a, b)
#    define BOOST_PP_TUPLE_DETAIL_ELEM_USE_OV_CAT_I(a, b) BOOST_PP_TUPLE_DETAIL_ELEM_USE_OV_CAT_II(a ## b)
#    define BOOST_PP_TUPLE_DETAIL_ELEM_USE_OV_CAT_II(res) res
#
#    define BOOST_PP_TUPLE_DETAIL_ELEM_USE_OVERLOAD(prefix,...) \
         BOOST_PP_TUPLE_DETAIL_ELEM_USE_OV_CAT(BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY()) \
         /**/
#
# else
#
#    define BOOST_PP_TUPLE_DETAIL_ELEM_USE_OVERLOAD(prefix,...) \
         BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__) \
         /**/
#
# endif
#
# /* BOOST_PP_TUPLE_DETAIL_REM_USE_OVERLOAD */
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_TUPLE_DETAIL_REM_USE_OV_CAT(a, b) BOOST_PP_TUPLE_DETAIL_REM_USE_OV_CAT_I(a, b)
#    define BOOST_PP_TUPLE_DETAIL_REM_USE_OV_CAT_I(a, b) BOOST_PP_TUPLE_DETAIL_REM_USE_OV_CAT_II(a ## b)
#    define BOOST_PP_TUPLE_DETAIL_REM_USE_OV_CAT_II(res) res
#
#    define BOOST_PP_TUPLE_DETAIL_REM_USE_OVERLOAD(prefix,...) \
         BOOST_PP_TUPLE_DETAIL_REM_USE_OV_CAT(BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY()) \
         /**/
#
# else
#
#    define BOOST_PP_TUPLE_DETAIL_REM_USE_OVERLOAD(prefix,...) \
         BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__) \
         /**/
#
# endif
#
# /* BOOST_PP_TUPLE_DETAIL_REVERSE_USE_OVERLOAD */
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_TUPLE_DETAIL_REVERSE_USE_OV_CAT(a, b) BOOST_PP_TUPLE_DETAIL_REVERSE_USE_OV_CAT_I(a, b)
#    define BOOST_PP_TUPLE_DETAIL_REVERSE_USE_OV_CAT_I(a, b) BOOST_PP_TUPLE_DETAIL_REVERSE_USE_OV_CAT_II(a ## b)
#    define BOOST_PP_TUPLE_DETAIL_REVERSE_USE_OV_CAT_II(res) res
#
#    define BOOST_PP_TUPLE_DETAIL_REVERSE_USE_OVERLOAD(prefix,...) \
         BOOST_PP_TUPLE_DETAIL_REVERSE_USE_OV_CAT(BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY()) \
         /**/
#
# else
#
#    define BOOST_PP_TUPLE_DETAIL_REVERSE_USE_OVERLOAD(prefix,...) \
         BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__) \
         /**/
#
# endif
#
# /* BOOST_PP_TUPLE_DETAIL_TO_ARRAY_USE_OVERLOAD */
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_TUPLE_DETAIL_TO_ARRAY_USE_OV_CAT(a, b) BOOST_PP_TUPLE_DETAIL_TO_ARRAY_USE_OV_CAT_I(a, b)
#    define BOOST_PP_TUPLE_DETAIL_TO_ARRAY_USE_OV_CAT_I(a, b) BOOST_PP_TUPLE_DETAIL_TO_ARRAY_USE_OV_CAT_II(a ## b)
#    define BOOST_PP_TUPLE_DETAIL_TO_ARRAY_USE_OV_CAT_II(res) res
#
#    define BOOST_PP_TUPLE_DETAIL_TO_ARRAY_USE_OVERLOAD(prefix,...) \
         BOOST_PP_TUPLE_DETAIL_TO_ARRAY_USE_OV_CAT(BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY()) \
         /**/
#
# else
#
#    define BOOST_PP_TUPLE_DETAIL_TO_ARRAY_USE_OVERLOAD(prefix,...) \
         BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__) \
         /**/
#
# endif
#
# /* BOOST_PP_TUPLE_DETAIL_TO_LIST_USE_OVERLOAD */
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_TUPLE_DETAIL_TO_LIST_USE_OV_CAT(a, b) BOOST_PP_TUPLE_DETAIL_TO_LIST_USE_OV_CAT_I(a, b)
#    define BOOST_PP_TUPLE_DETAIL_TO_LIST_USE_OV_CAT_I(a, b) BOOST_PP_TUPLE_DETAIL_TO_LIST_USE_OV_CAT_II(a ## b)
#    define BOOST_PP_TUPLE_DETAIL_TO_LIST_USE_OV_CAT_II(res) res
#
#    define BOOST_PP_TUPLE_DETAIL_TO_LIST_USE_OVERLOAD(prefix,...) \
         BOOST_PP_TUPLE_DETAIL_TO_LIST_USE_OV_CAT(BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY()) \
         /**/
#
# else
#
#    define BOOST_PP_TUPLE_DETAIL_TO_LIST_USE_OVERLOAD(prefix,...) \
         BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__) \
         /**/
#
# endif
#
# /* BOOST_PP_TUPLE_DETAIL_TO_SEQ_USE_OVERLOAD */
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_TUPLE_DETAIL_TO_SEQ_USE_OV_CAT(a, b) BOOST_PP_TUPLE_DETAIL_TO_SEQ_USE_OV_CAT_I(a, b)
#    define BOOST_PP_TUPLE_DETAIL_TO_SEQ_USE_OV_CAT_I(a, b) BOOST_PP_TUPLE_DETAIL_TO_SEQ_USE_OV_CAT_II(a ## b)
#    define BOOST_PP_TUPLE_DETAIL_TO_SEQ_USE_OV_CAT_II(res) res
#
#    define BOOST_PP_TUPLE_DETAIL_TO_SEQ_USE_OVERLOAD(prefix,...) \
         BOOST_PP_TUPLE_DETAIL_TO_SEQ_USE_OV_CAT(BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY()) \
         /**/
#
# else
#
#    define BOOST_PP_TUPLE_DETAIL_TO_SEQ_USE_OVERLOAD(prefix,...) \
         BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__) \
         /**/
#
# endif
#
# endif /* BOOST_PP_VARIADICS */
# endif /* BOOST_PREPROCESSOR_TUPLE_USE_OVERLOAD_HPP */
