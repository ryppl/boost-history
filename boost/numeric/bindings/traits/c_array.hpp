// (-*- C++ -*- header)  

#ifndef BOOST_BINDINGS_TRAITS_C_ARRAY_H
#define BOOST_BINDINGS_TRAITS_C_ARRAY_H

#include <boost/numeric/bindings/traits/traits.hpp>

namespace boost { namespace numeric { namespace bindings {

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

}}}  

#endif 
