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
# ifndef BOOST_PREPROCESSOR_VARIADIC_DETAIL_VD_CAT_HPP
# define BOOST_PREPROCESSOR_VARIADIC_DETAIL_VD_CAT_HPP
#
# include <boost/preprocessor/config/config.hpp>
#
# /* BOOST_PP_VD_CAT */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_VD_CAT(a, b) BOOST_PP_VD_CAT_I(a, b)
# else
#    define BOOST_PP_VD_CAT(a, b) BOOST_PP_VD_CAT_OO((a, b))
#    define BOOST_PP_VD_CAT_OO(par) BOOST_PP_VD_CAT_I ## par
# endif
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_VD_CAT_I(a, b) a ## b
# else
#    define BOOST_PP_VD_CAT_I(a, b) BOOST_PP_VD_CAT_II(a ## b)
#    define BOOST_PP_VD_CAT_II(res) res
# endif
#
#endif /* BOOST_PREPROCESSOR_VARIADIC_DETAIL_VD_CAT_HPP */
