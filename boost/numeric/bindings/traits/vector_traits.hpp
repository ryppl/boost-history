/*
 * 
 * Copyright (c) 2002, 2003 Kresimir Fresl and Toon Knapen 
 *
 * Permission to copy, modify, use and distribute this software 
 * for any non-commercial or commercial purpose is granted provided 
 * that this license appear on all copies of the software source code.
 *
 * Authors assume no responsibility whatsoever for its use and makes 
 * no guarantees about its quality, correctness or reliability.
 *
 * First author acknowledges the support of the Faculty of Civil 
 * Engineering, University of Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_VECTOR_TRAITS_HPP
#define BOOST_NUMERIC_BINDINGS_TRAITS_VECTOR_TRAITS_HPP

#include <boost/numeric/bindings/traits/config.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  /// default_vector_traits is just a base-class that can be
  /// used in the default vector_traits and the different
  /// specialisation to automatically define most of the
  /// functions.
  ///
  /// portability:
  /// no second template which is set to V::value_type by default since
  /// not portable
  template <typename V, typename T >
  struct default_vector_traits {
#ifndef BOOST_NO_CV_TEMPLATE_TEMPLATES
    typedef T value_type; 
    typedef T*   pointer; 
    // assumption: iterator==pointer
    // .. e.g. ublas::(un)bounded_array 
    static pointer storage (V& v) { return v.begin(); }
#endif // BOOST_NO_CV_TEMPLATE_TEMPLATES
    static int size (V& v) { return v.size(); } 
    static int stride (V& v) { return 1; } 
  }; 

#ifndef BOOST_NO_CV_TEMPLATE_TEMPLATES

  /// Specialisation for const vectors, mainly because
  /// of the effect on the pointer_type
  template <typename V, typename T>
  struct default_vector_traits<V const, T> {
    typedef T value_type; 
    typedef T const* pointer; 
    static pointer storage (V const& v) { return v.begin(); }
    static int size (V const& v) { return v.size(); } 
    static int stride (V const& v) { return 1; } 
  }; 

#endif // BOOST_NO_CV_TEMPLATE_TEMPLATES

  // vector_traits<> generic version: 
  template <typename V>
  struct vector_traits : default_vector_traits<V, typename V::value_type > {}; 


  ///////////////////////////
  //
  // free accessor functions: 
  //
  ///////////////////////////

  template <typename V>
  inline 
  typename vector_traits<V>::pointer vector_storage (V& v) { 
    return vector_traits<V>::storage (v); 
  }

  template <typename V>
  inline
  int vector_size (V& v) { 
    return vector_traits<V>::size (v); 
  }

  template <typename V>
  inline
  int vector_stride (V& v) { 
    return vector_traits<V>::stride (v); 
  }

}}}}

#endif // BOOST_NUMERIC_BINDINGS_TRAITS_VECTOR_TRAITS_HPP
