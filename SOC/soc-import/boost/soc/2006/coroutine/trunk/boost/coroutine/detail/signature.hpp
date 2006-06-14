//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_SIGNATURE_HPP_20060609
#define BOOST_COROUTINE_DETAIL_SIGNATURE_HPP_20060609
#include <boost/preprocessor/repetition.hpp>
namespace boost{ namespace coroutines{ namespace detail{
  /*
   * Derived from an @p mpl::vector describing
   * @p Function arguments types.
   */
  template<typename Function>
  struct signature;

  /*
   * Generate specializations for the @p signature trait class.
   */
#define BOOST_COROUTINE_SIGNATURE_GENERATOR(z, n, unused)                    \
  template <class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class A)>   \
  struct signature<R( BOOST_PP_ENUM_PARAMS(n,A) ) >              \
    : boost::mpl::BOOST_PP_CAT(vector,n)<                        \
    BOOST_PP_ENUM_PARAMS(n,A)                                    \
    > {};                                                        \
/**/

  
  BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX, BOOST_COROUTINE_SIGNATURE_GENERATOR, ~);
#undef BOOST_COROUTINE_SIGNATURE_GENERATOR
} } }
#endif
