# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2011.
#  *     (C) Copyright Edward Diener 2011.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_FACILITIES_OVERLOAD_HPP
# define BOOST_PREPROCESSOR_FACILITIES_OVERLOAD_HPP
#
# include <boost/preprocessor/config/variadics.hpp>
#
#if BOOST_PP_VARIADICS
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/variadic/size.hpp>
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
# include <boost/preprocessor/facilities/empty.hpp>
# endif
#
# /* BOOST_PP_OVERLOAD */
#
#define BOOST_PP_OVERLOAD(prefix, ...) \
  BOOST_PP_OV_CAT(prefix, BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)) \
/**/
#
# /* BOOST_PP_CALL_OVERLOAD */
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#
# define BOOST_PP_CALL_OVERLOAD(prefix, ...) \
  BOOST_PP_CALL_OV_CAT(BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY()) \
/**/
#
# else
#
# define BOOST_PP_CALL_OVERLOAD(prefix, ...) \
  BOOST_PP_OVERLOAD(prefix, __VA_ARGS__)(__VA_ARGS__) \
/**/
#
# endif
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_OV_CAT(a, b) BOOST_PP_OV_CAT_I(a, b)
#    define BOOST_PP_CALL_OV_CAT(a, b) BOOST_PP_CALL_OV_CAT_I(a, b)
# else
#    define BOOST_PP_OV_CAT(a, b) BOOST_PP_OV_CAT_OO((a, b))
#    define BOOST_PP_OV_CAT_OO(par) BOOST_PP_OV_CAT_I ## par
# endif
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_OV_CAT_I(a, b) a ## b
# else
#    define BOOST_PP_OV_CAT_I(a, b) BOOST_PP_OV_CAT_II(a ## b)
#    define BOOST_PP_OV_CAT_II(res) res
#    define BOOST_PP_CALL_OV_CAT_I(a, b) BOOST_PP_CALL_OV_CAT_II(a ## b)
#    define BOOST_PP_CALL_OV_CAT_II(res) res
# endif
#
#endif /* BOOST_PP_VARIADICS */
#endif /* BOOST_PREPROCESSOR_FACILITIES_OVERLOAD_HPP */
