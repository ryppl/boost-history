// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_BLAS_TRAITS_C_ARRAY_H
#define BOOST_BLASBINDINGS_BLAS_TRAITS_C_ARRAY_H

#include <boost/numeric/blasbindings/blas_traits.hpp>
#include <boost/numeric/blasbindings/storage_traits/c_array.hpp> 

namespace boost { namespace numeric { namespace blasbindings {

  template <typename T, std::size_t N> 
  struct blas_vector_traits<T[N]> {
    typedef T value_type; 
    typedef typename storage_traits<T[N]>::pointer pointer; 
    static pointer storage (T (&a)[N]) {
      return storage_traits<T[N]>::storage (a); 
    }
    static int size (T (&)[N]) { return N; } 
    static int stride (T (&)[N]) { return 1; } 
  }; 
  template <typename T, std::size_t N> 
  struct blas_vector_traits<T const [N]> {
    typedef T value_type; 
    typedef typename storage_traits<T const [N]>::pointer pointer; 
    static pointer storage (T const (&a)[N]) {
      return storage_traits<T const [N]>::storage (a); 
    }
    static int size (T const (&)[N]) { return N; } 
    static int stride (T const (&)[N]) { return 1; } 
  }; 

}}}  

#endif // BOOST_BLASBINDINGS_BLAS_TRAITS_C_ARRAY_H
