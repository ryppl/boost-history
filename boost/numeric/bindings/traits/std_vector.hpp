// (-*- C++ -*- header)  

#ifndef BOOST_BINDINGS_TRAITS_STD_VECTOR_H
#define BOOST_BINDINGS_TRAITS_STD_VECTOR_H

#include <boost/numeric/bindings/traits/traits.hpp>
#include <vector>

namespace boost { namespace numeric { namespace bindings {

  // std::vector<>
  template <typename T, typename Alloc>
  struct vector_traits<std::vector<T, Alloc> > 
  : default_vector_traits<std::vector<T, Alloc> >
  {
    // g++ 3.2 doesn't accept, como does:
    //   using typename 
    //   default_vector_traits<std::vector<T, Alloc> >::pointer; 
    // therefore: 
    //   typedef typename 
    //   default_vector_traits<std::vector<T, Alloc> >::pointer pointer; 
    // but `T*' is probably the best solution (certainly the simplest)
    static T* storage (std::vector<T, Alloc>& v) {
      return &v.front(); 
    }
  }; 

  template <typename T, typename Alloc>
  struct vector_traits<std::vector<T, Alloc> const> 
  : default_vector_traits<std::vector<T, Alloc> const>
  {
    static T const* storage (std::vector<T, Alloc> const& v) {
      return &v.front(); 
    }
  }; 

}}}  

#endif 
