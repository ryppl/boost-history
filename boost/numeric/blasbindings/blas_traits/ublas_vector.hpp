// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_BLAS_TRAITS_UBLAS_VECTOR_H
#define BOOST_BLASBINDINGS_BLAS_TRAITS_UBLAS_VECTOR_H

#include <boost/numeric/blasbindings/blas_traits.hpp>
#include <boost/numeric/blasbindings/storage_traits/ublas_vector.hpp> 

namespace boost { namespace numeric { namespace blasbindings {

  // ublas::vector<>
  // generic version is OK 

  // ublas::vector_range<>
  template <typename V>
  struct blas_vector_traits<boost::numeric::ublas::vector_range<V> > {
    typedef boost::numeric::ublas::vector_range<V> vector_type; 
    typedef typename vector_type::value_type value_type;
    typedef typename storage_traits<vector_type>::pointer pointer; 
    static pointer storage (vector_type& vr) {
      return storage_traits<vector_type>::storage (vr); 
    }
    static int size (vector_type& vr) { return vr.size(); } 
    static int stride (vector_type& vr) { 
      return storage_traits<vector_type>::stride (vr); 
    } 
  }; 
  template <typename V>
  struct blas_vector_traits<boost::numeric::ublas::vector_range<V> const> {
    typedef boost::numeric::ublas::vector_range<V> vector_type; 
    typedef typename vector_type::value_type value_type;
    typedef typename storage_traits<vector_type const>::pointer pointer; 
    static pointer storage (vector_type const& cvr) {
      return storage_traits<vector_type const>::storage (cvr); 
    }
    static int size (vector_type const& cvr) { return cvr.size(); } 
    static int stride (vector_type const& cvr) { 
      return storage_traits<vector_type const>::stride (cvr); 
    } 
  }; 

  // ublas::vector_slice<>
  template <typename V>
  struct blas_vector_traits<boost::numeric::ublas::vector_slice<V> > {
    typedef boost::numeric::ublas::vector_slice<V> vector_type; 
    typedef typename vector_type::value_type value_type;
    typedef typename storage_traits<vector_type>::pointer pointer; 
    static pointer storage (vector_type& vs) {
      return storage_traits<vector_type>::storage (vs); 
    }
    static int size (vector_type& vs) { return vs.size(); } 
    static int stride (vector_type& vs) { 
      return storage_traits<vector_type>::stride (vs); 
    } 
  }; 
  template <typename V>
  struct blas_vector_traits<boost::numeric::ublas::vector_slice<V> const> {
    typedef boost::numeric::ublas::vector_slice<V> vector_type; 
    typedef typename vector_type::value_type value_type;
    typedef typename storage_traits<vector_type const>::pointer pointer; 
    static pointer storage (vector_type const& cvs) {
      return storage_traits<vector_type const>::storage (cvs); 
    }
    static int size (vector_type const& cvs) { return cvs.size(); } 
    static int stride (vector_type const& cvs) { 
      return storage_traits<vector_type const>::stride (cvs); 
    } 
  }; 

}}}  

#endif // BOOST_BLASBINDINGS_BLAS_TRAITS_UBLAS_VECTOR_H
