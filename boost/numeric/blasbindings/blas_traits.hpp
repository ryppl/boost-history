// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_BLAS_TRAITS_H
#define BOOST_BLASBINDINGS_BLAS_TRAITS_H

#include <boost/numeric/blasbindings/storage_traits.hpp>

namespace boost { namespace numeric { namespace blasbindings {

  template <typename V>
  struct blas_vector_traits {
    typedef typename V::value_type value_type; 
    typedef typename storage_traits<V>::pointer pointer; 
    static pointer storage (V& v) {
      return storage_traits<V>::storage (v); 
    }
    static int size (V& v) { return v.size(); } 
    static int stride (V& v) { return 1; } 
  }; 

  template <typename M>
  struct blas_matrix_traits {
    typedef typename M::value_type value_type; 
    typedef typename storage_traits<M>::pointer pointer; 
    static pointer storage (M& m) {
      return storage_traits<M>::storage (m); 
    }
    static int size1 (M& m) { return m.size1(); } 
    static int size2 (M& m) { return m.size2(); } 
  }; 

}}}  

#endif // BOOST_BLASBINDINGS_BLAS_TRAITS_H
