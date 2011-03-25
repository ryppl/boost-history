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
# ifndef BOOST_PREPROCESSOR_VARIADIC_TO_ARRAY_HPP
# define BOOST_PREPROCESSOR_VARIADIC_TO_ARRAY_HPP
#
# include <boost/preprocessor/config/variadics.hpp>
#
#if BOOST_PP_VARIADICS
#
#include "detail/to_array.hpp"
#
#define BOOST_PP_TO_ARRAY(...) \
  BOOST_PP_VARIADIC_DETAIL_TO_ARRAY(__VA_ARGS__) \
/**/
#endif // BOOST_PP_VARIADICS
#endif // BOOST_PREPROCESSOR_VARIADIC_TO_ARRAY_HPP
