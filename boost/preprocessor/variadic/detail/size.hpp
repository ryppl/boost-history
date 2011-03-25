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
# ifndef BOOST_PREPROCESSOR_VARIADIC_DETAIL_SIZE_HPP
# define BOOST_PREPROCESSOR_VARIADIC_DETAIL_SIZE_HPP
#
# include <boost/preprocessor/config/variadics.hpp>
#
#if BOOST_PP_VARIADICS
#
# include <boost/preprocessor/config/config.hpp>
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
# include <boost/preprocessor/arithmetic/add.hpp>
# endif
#
#define BOOST_PP_VARIADIC_DETAIL_ARG_N( \
  A1,A2,A3,A4,A5,A6,A7,A8,A9,A10, \
  A11,A12,A13,A14,A15,A16,A17,A18,A19,A20, \
  A21,A22,A23,A24,A25,A26,A27,A28,A29,A30, \
  A31,A32,A33,A34,A35,A36,A37,A38,A39,A40, \
  A41,A42,A43,A44,A45,A46,A47,A48,A49,A50, \
  A51,A52,A53,A54,A55,A56,A57,A58,A59,A60, \
  A61,A62,A63,N,...) N \
/**/
#define BOOST_PP_VARIADIC_DETAIL_RSEQ_N() \
  63,62,61,60, \
  59,58,57,56,55,54,53,52,51,50, \
  49,48,47,46,45,44,43,42,41,40, \
  39,38,37,36,35,34,33,32,31,30, \
  29,28,27,26,25,24,23,22,21,20, \
  19,18,17,16,15,14,13,12,11,10, \
  9,8,7,6,5,4,3,2,1,0 \
/**/
#define BOOST_PP_VARIADIC_DETAIL_APPLY_I(macro, args) \
  macro args \
/**/
#define BOOST_PP_VARIADIC_DETAIL_APPLY(macro, args) \
  BOOST_PP_VARIADIC_DETAIL_APPLY_I(macro, args) \
/**/
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#define BOOST_PP_VARIADIC_DETAIL_SIZE(...) \
  BOOST_PP_ADD(BOOST_PP_VARIADIC_DETAIL_APPLY(BOOST_PP_VARIADIC_DETAIL_ARG_N, (__VA_ARGS__, BOOST_PP_VARIADIC_DETAIL_RSEQ_N())),0) \
/**/
#else
#define BOOST_PP_VARIADIC_DETAIL_SIZE(...) \
  BOOST_PP_VARIADIC_DETAIL_APPLY(BOOST_PP_VARIADIC_DETAIL_ARG_N, (__VA_ARGS__, BOOST_PP_VARIADIC_DETAIL_RSEQ_N())) \
/**/
#endif
#
#endif // BOOST_PP_VARIADICS
#endif // BOOST_PREPROCESSOR_VARIADIC_DETAIL_SIZE_HPP
