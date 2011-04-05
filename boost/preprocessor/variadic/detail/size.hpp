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
# include <boost/preprocessor/variadic/detail/apply_var.hpp>
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
# include <boost/preprocessor/variadic/detail/vd_cat.hpp>
# include <boost/preprocessor/facilities/empty.hpp>
# endif
#
/*
  The maximum size currently supported for variadic 
  data in the library is 25, as given by the following
  two macros. If we change this we also
  need to change it in the internal copy ( under a different 
  name to avoid recursion ) of the same functionality in 
  the header file 'facilities/overload.hpp'.
*/
#define BOOST_PP_VARIADIC_DETAIL_ARG_N( \
  A1,A2,A3,A4,A5,A6,A7,A8,A9,A10, \
  A11,A12,A13,A14,A15,A16,A17,A18,A19,A20, \
  A21,A22,A23,A24,N,...) N \
/**/
#define BOOST_PP_VARIADIC_DETAIL_RSEQ_N() \
  24,23,22,21,20, \
  19,18,17,16,15,14,13,12,11,10, \
  9,8,7,6,5,4,3,2,1,0 \
/**/
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#define BOOST_PP_VARIADIC_DETAIL_SIZE(...) \
  BOOST_PP_VD_CAT(BOOST_PP_VARIADIC_DETAIL_APPLY_VAR(BOOST_PP_VARIADIC_DETAIL_ARG_N, (__VA_ARGS__, BOOST_PP_VARIADIC_DETAIL_RSEQ_N())),BOOST_PP_EMPTY()) \
/**/
#else
#define BOOST_PP_VARIADIC_DETAIL_SIZE(...) \
  BOOST_PP_VARIADIC_DETAIL_APPLY_VAR(BOOST_PP_VARIADIC_DETAIL_ARG_N, (__VA_ARGS__, BOOST_PP_VARIADIC_DETAIL_RSEQ_N())) \
/**/
#endif
#
#endif /* BOOST_PP_VARIADICS */
#endif /* BOOST_PREPROCESSOR_VARIADIC_DETAIL_SIZE_HPP */
