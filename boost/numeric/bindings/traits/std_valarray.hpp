// (-*- C++ -*- header)  

/*
 * 
 * Copyright (c) Kresimir Fresl 2002 
 *
 * Permission to copy, modify, use and distribute this software 
 * for any non-commercial or commercial purpose is granted provided 
 * that this license appear on all copies of the software source code.
 *
 * Author assumes no responsibility whatsoever for its use and makes 
 * no guarantees about its quality, correctness or reliability.
 *
 * Author acknowledges the support of the Faculty of Civil Engineering, 
 * University of Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_STD_VALARRAY_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_STD_VALARRAY_H

#include <boost/numeric/bindings/traits/config.hpp> 

#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#include <boost/numeric/bindings/traits/vector_traits.hpp>
#include <valarray>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // std::valarray<>
  template <typename T> 
  struct vector_traits<std::valarray<T> > 
  : default_vector_traits<std::valarray<T> > 
  {
    static T* storage (std::valarray<T>& va) { return &va[0]; }
  }; 

  template <typename T> 
  struct vector_traits<std::valarray<T> const> 
  : default_vector_traits<std::valarray<T> const> 
  {
    static T const* storage (std::valarray<T> const& va) {
      std::valarray<T>& ncva = const_cast<std::valarray<T>&> (va);
      return &ncva[0];
    }
  }; 

}}}}  

#else // BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#error with your compiler std::valarray<> cannot be used in bindings

#endif // BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#endif // BOOST_NUMERIC_BINDINGS_TRAITS_STD_VALARRAY_H
