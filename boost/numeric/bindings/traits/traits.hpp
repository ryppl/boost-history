// (-*- C++ -*- header)  

#ifndef BOOST_BINDINGS_TRAITS_H
#define BOOST_BINDINGS_TRAITS_H

#include <cstddef> 

namespace boost { namespace numeric { namespace bindings { 

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

}}}  

#endif // BOOST_BINDINGS_TRAITS_H
