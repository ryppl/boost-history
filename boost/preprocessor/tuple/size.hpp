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
# ifndef BOOST_PREPROCESSOR_TUPLE_SIZE_HPP
# define BOOST_PREPROCESSOR_TUPLE_SIZE_HPP
#
# include <boost/preprocessor/config/variadics.hpp>
#
#if BOOST_PP_VARIADICS
#
# include <boost/preprocessor/tuple/enum.hpp>
# include <boost/preprocessor/variadic/size.hpp>
#
# /* BOOST_PP_TUPLE_SIZE */
#
#define BOOST_PP_TUPLE_SIZE(tuple) BOOST_PP_TUPLE_SIZE_DETAIL(tuple)
#define BOOST_PP_TUPLE_SIZE_DETAIL(tuple) BOOST_PP_VARIADIC_SIZE(BOOST_PP_TUPLE_ENUM(tuple))
#
#endif // BOOST_PP_VARIADICS
#endif // BOOST_PREPROCESSOR_TUPLE_SIZE_HPP
