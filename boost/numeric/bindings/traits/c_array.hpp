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

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_C_ARRAY_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_C_ARRAY_H

#include <boost/numeric/bindings/traits/vector_traits.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // built-in array
  template <typename T, std::size_t N> 
  struct vector_traits<T[N]> 
  : default_vector_traits <T[N], T> 
  {
    static T* storage (T (&a)[N]) { return a; }
    static int size (T (&)[N]) { return N; } 
  }; 

  template <typename T, std::size_t N> 
  struct vector_traits<T const [N]> 
  : default_vector_traits <T const [N], T> 
  {
    static T const* storage (T const (&a)[N]) { return a; } 
    static int size (T const (&)[N]) { return N; } 
  }; 

}}}}

#endif 
