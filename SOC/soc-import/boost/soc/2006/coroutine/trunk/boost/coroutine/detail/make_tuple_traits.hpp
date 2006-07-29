//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_MAKE_TUPLE_TRAITS_HPP_20060609
#define BOOST_COROUTINE_DETAIL_MAKE_TUPLE_TRAITS_HPP_20060609
#include <boost/preprocessor/repetition.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/coroutine/detail/arg_max.hpp>
#include <boost/coroutine/tuple_traits.hpp>

namespace boost { namespace coroutines { namespace detail {
  /*
   * Given a mpl::vector, returns a nullary metafunction
   * describing a tuple of all types in the vector.
   * NOTE this is just wrong because it should work for all mpl
   * sequences, not just vectors. But it is in detail, so leave it
   * as is. Eventually it will be replaced by Boost.Fusion.
   * @p type is a tuple of all types in TypeList.
   * TypeList is one of mpl::vector0, mpl::vector1, etc.
   */
  template<typename TypeList>
  struct make_tuple_traits;

#define BOOST_COROUTINE_MAKE_TUPLE_TRAITS_GENERATOR(z, n, unused)       \
    template<BOOST_PP_ENUM_PARAMS(n, class A)>                   \
    struct make_tuple_traits<BOOST_PP_CAT(boost::mpl::vector, n)        \
        <BOOST_PP_ENUM_PARAMS(n, A)> >{                          \
        typedef tuple_traits<BOOST_PP_ENUM_PARAMS(n, A)> type;   \
    };                                                           \
/**/

  /**
   * Generate specializations of make_tuple
   * @note This could be done more elegantly with a recursive metafunction, 
   * but this is simpler and works well anyway.
   */
  BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX, 
		  BOOST_COROUTINE_MAKE_TUPLE_TRAITS_GENERATOR, ~);
#undef BOOST_COROUTINE_MAKE_TUPLE_TRAITS_GENERATOR

} } }
#endif 
