// (-*- C++ -*- header)  

#ifndef BOOST_BINDINGS_TRAITS_UBLAS_VECTOR_H
#define BOOST_BINDINGS_TRAITS_UBLAS_VECTOR_H

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace boost { namespace numeric { namespace bindings {

  // ublas::vector<>
  template <typename T, typename ArrT>
  struct vector_traits<boost::numeric::ublas::vector<T, ArrT> > 
  : default_vector_traits<boost::numeric::ublas::vector<T, ArrT> > 
  {
    typedef boost::numeric::ublas::vector<T, ArrT> vector_type;
    static T* storage (vector_type& v) {
      return vector_traits<ArrT>::storage (v.data()); 
    }
  }; 

  template <typename T, typename ArrT>
  struct vector_traits<boost::numeric::ublas::vector<T, ArrT> const> 
  : default_vector_traits<boost::numeric::ublas::vector<T, ArrT> const> 
  {
    typedef boost::numeric::ublas::vector<T, ArrT> vector_type;
    static T const* storage (vector_type const& v) {
      return vector_traits<ArrT const>::storage (v.data()); 
    }
  }; 

  // ublas::vector_range<>
  template <typename V>
  struct vector_traits<boost::numeric::ublas::vector_range<V> > 
  : default_vector_traits<boost::numeric::ublas::vector_range<V> > 
  {
    typedef boost::numeric::ublas::vector_range<V> vector_type; 
    typedef typename default_vector_traits<vector_type>::pointer pointer; 
    static pointer storage (vector_type& vr) {
      pointer ptr = vector_traits<V>::storage (vr.data()); 
      ptr += vr.start() * vector_traits<V>::stride (vr.data());
      return ptr; 
    }
    static int stride (vector_type& vr) {
      return vector_traits<V>::stride (vr.data()); 
    }
  }; 

  template <typename V>
  struct vector_traits<boost::numeric::ublas::vector_range<V> const> 
  : default_vector_traits<boost::numeric::ublas::vector_range<V> const> 
  {
    typedef boost::numeric::ublas::vector_range<V> vector_type; 
    typedef 
      typename default_vector_traits<vector_type const>::pointer pointer; 
    static pointer storage (vector_type const& vr) {
      pointer ptr = vector_traits<V const>::storage (vr.data()); 
      ptr += vr.start() * vector_traits<V const>::stride (vr.data());
      return ptr; 
    }
    static int stride (vector_type const& vr) {
      return vector_traits<V const>::stride (vr.data()); 
    }
  }; 

  // ublas::vector_slice<>
  template <typename V>
  struct vector_traits<boost::numeric::ublas::vector_slice<V> > 
  : default_vector_traits<boost::numeric::ublas::vector_slice<V> > 
  {
    typedef boost::numeric::ublas::vector_slice<V> vector_type; 
    typedef typename default_vector_traits<vector_type>::pointer pointer; 
    static pointer storage (vector_type& vs) {
      pointer ptr = vector_traits<V>::storage (vs.data()); 
      ptr += vs.start() * vector_traits<V>::stride (vs.data());
      return ptr; 
    }
    static int stride (vector_type& vs) {
      return vs.stride() * vector_traits<V>::stride (vs.data()); 
    }
  }; 

  template <typename V>
  struct vector_traits<boost::numeric::ublas::vector_slice<V> const> 
  : default_vector_traits<boost::numeric::ublas::vector_slice<V> const> 
  {
    typedef boost::numeric::ublas::vector_slice<V> vector_type; 
    typedef 
      typename default_vector_traits<vector_type const>::pointer pointer; 
    static pointer storage (vector_type const& vs) {
      pointer ptr = vector_traits<V const>::storage (vs.data()); 
      ptr += vs.start() * vector_traits<V const>::stride (vs.data());
      return ptr; 
    }
    static int stride (vector_type const& vs) {
      return vs.stride() * vector_traits<V const>::stride (vs.data()); 
    }
  }; 

}}}  

#endif // BOOST_BINDINGS_TRAITS_UBLAS_VECTOR_H
