//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_MAKE_TUPLE_HPP_20060609
#define BOOST_COROUTINE_DETAIL_MAKE_TUPLE_HPP_20060609
#include <boost/preprocessor/repetition.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/coroutine/tuple.hpp>
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
  struct make_tuple;

#define BOOST_COROUTINE_MAKE_TUPLE_GENERATOR(z, n, unused)       \
    template<BOOST_PP_ENUM_PARAMS(n, class A)>                   \
    struct make_tuple<BOOST_PP_CAT(boost::mpl::vector, n)        \
        <BOOST_PP_ENUM_PARAMS(n, A)> >{                          \
        typedef tuple<BOOST_PP_ENUM_PARAMS(n, A)> type;   \
    };                                                           \
/**/

  /**
   * Generate specializations of make_tuple
   * @note This could be done more elegantly with a recursive metafunction, 
   * but this is simpler and works well anyway.
   */
  BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX, 
		  BOOST_COROUTINE_MAKE_TUPLE_GENERATOR, ~);
#undef BOOST_COROUTINE_MAKE_TUPLE_GENERATOR

#if 0
  /**
   * Generic remove tuple for multielements tuples. Identity function.
   */
  template<typename Tuple>
  inline
  Tuple remove_tuple(const Tuple& t) { 
    return t;
  }
 
  /**
   * Overload of @p remove_tuple for single element tuples.
   * Returns the first (and only) element.
   */
  template<typename Elem>
  inline
  Elem remove_tuple(const boost::tuple<Elem>& t) {
    return boost::tuples::get<0>(t);
  }
  
  /**
   * Overload of @p remove_tuple for nullary tuples.
   */
  inline
  void remove_tuple(boost::tuple<>) {}
#endif
} } }
#endif 
