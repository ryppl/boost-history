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
  struct matrix_traits 
  {
    // typedefs:
    //   value_type
    //   pointer
    //   ordering_type
    // static functions:
    //   pointer storage()
    //   int size1()
    //   int size2()
    //   int leading_dimension()
  }; 

  // storage ordering tags: 
  struct row_major_t {};
  struct column_major_t {};

  // upper/lower tags:
  struct upper_t {};
  struct lower_t {};

  //
  // Free functions that automatically rely on the correct traits class
  //
  
  template < typename vector_type >
  typename vector_type::value_type* vector_storage(vector_type& v)
  { return vector_traits< vector_type >::storage( v ) ; }

  template < typename vector_type >
  const typename vector_type::value_type* vector_storage(const vector_type& v)
  { return vector_traits< const vector_type >::storage( v ) ; }

  template < typename vector_type >
  int size(vector_type& v)
  { return vector_traits< vector_type >::size( v ) ; }

  template < typename vector_type >
  int stride(vector_type& v)
  { return vector_traits< vector_type >::stride( v ) ; }

  template < typename matrix_type >
  typename matrix_type::value_type* matrix_storage(matrix_type& m)
  { return matrix_traits< matrix_type >::storage( m ) ; }
  
  template < typename matrix_type >
  const typename matrix_type::value_type* matrix_storage(const matrix_type& m)
  { return matrix_traits< const matrix_type >::storage( m ) ; }
  
  template < typename matrix_type >
  int size1(matrix_type& m)
  { return matrix_traits< matrix_type >::size1( m ) ; }
  
  template < typename matrix_type >
  int size2(matrix_type& m)
  { return matrix_traits< matrix_type >::size2( m ) ; }
  
  template < typename matrix_type >
  int stride1(matrix_type& m)
  { return matrix_traits< matrix_type >::stride1( m ) ; }
  
  template < typename matrix_type >
  int stride2(matrix_type& m)
  { return matrix_traits< matrix_type >::stride2( m ) ; }
  
  template < typename matrix_type >
  int leading_dimension(matrix_type& m)
  { return matrix_traits< matrix_type >::leading_dimension( m ) ; }
  
}}}}  

#endif 
