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
# ifndef BOOST_PREPROCESSOR_VARIADIC_TO_SEQ_HPP
# define BOOST_PREPROCESSOR_VARIADIC_TO_SEQ_HPP
#
# include <boost/preprocessor/config/variadics.hpp>
#
#if BOOST_PP_VARIADICS
#
#include "detail/to_seq.hpp"
#
#define BOOST_PP_TO_SEQ(...) \
  BOOST_PP_VARIADIC_DETAIL_TO_SEQ(__VA_ARGS__) \
/**/
#endif // BOOST_PP_VARIADICS
#endif // BOOST_PREPROCESSOR_VARIADIC_TO_SEQ_HPP
