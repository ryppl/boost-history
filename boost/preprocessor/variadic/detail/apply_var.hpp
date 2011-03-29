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
# ifndef BOOST_PREPROCESSOR_VARIADIC_DETAIL_APPLYVAR_HPP
# define BOOST_PREPROCESSOR_VARIADIC_DETAIL_APPLYVAR_HPP
#
#define BOOST_PP_VARIADIC_DETAIL_APPLY_VAR_I(macro, args) \
  macro args \
/**/
#define BOOST_PP_VARIADIC_DETAIL_APPLY_VAR(macro, args) \
  BOOST_PP_VARIADIC_DETAIL_APPLY_VAR_I(macro, args) \
/**/
#
#endif // BOOST_PREPROCESSOR_VARIADIC_DETAIL_APPLYVAR_HPP
