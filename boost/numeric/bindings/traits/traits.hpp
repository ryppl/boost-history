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
 * Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_H

#include <cstddef> 

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // vector_traits<> base:
  // .. implementation -- do not use it directly
  template <typename V, typename T = typename V::value_type>
  struct default_vector_traits {
    typedef T value_type; 
    typedef T* pointer; 
    // assumption: iterator==pointer
    // .. e.g. ublas::(un)bounded_array 
    static pointer storage (V& v) { return v.begin(); }
    static int size (V& v) { return v.size(); } 
    static int stride (V& v) { return 1; } 
  }; 

  template <typename V, typename T>
  struct default_vector_traits<V const, T> {
    typedef T value_type; 
    typedef T const* pointer; 
    static pointer storage (V const& v) { return v.begin(); }
    static int size (V const& v) { return v.size(); } 
    static int stride (V const& v) { return 1; } 
  }; 

  // vector_traits<> generic version: 
  template <typename V>
  struct vector_traits : default_vector_traits<V> {}; 


  // matrix_traits<> generic version: 
  template <typename M>
  struct matrix_traits {}; 

  // storage ordering tags: 
  struct row_major_t {};
  struct column_major_t {};

}}}}  

#endif 
