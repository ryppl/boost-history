// (-*- C++ -*- header)  

#ifndef BOOST_BINDINGS_TRAITS_STD_VALARRAY_H
#define BOOST_BINDINGS_TRAITS_STD_VALARRAY_H

#include <boost/numeric/bindings/traits/traits.hpp>
#include <valarray>

namespace boost { namespace numeric { namespace bindings {

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

}}}  

#endif 
