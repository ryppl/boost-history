// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_STORAGE_TRAITS_UBLAS_VECTOR_H
#define BOOST_BLASBINDINGS_STORAGE_TRAITS_UBLAS_VECTOR_H

#include <cstddef> 
#include <boost/numeric/blasbindings/storage_traits.hpp> 
#include <boost/numeric/ublas/vector.hpp>

namespace boost { namespace numeric { namespace blasbindings {

  // ublas::vector<> 
  template <typename T, typename ArrT>
  struct storage_traits<boost::numeric::ublas::vector<T, ArrT> > {
    typedef boost::numeric::ublas::vector<T, ArrT> vector_type;
    typedef T* pointer; 
    static pointer storage (vector_type& v) {
      return storage_traits<ArrT>::storage (v.data()); 
    }
    static std::ptrdiff_t stride (vector_type& v) { return 1; }
  }; 
  template <typename T, typename ArrT>
  struct storage_traits<boost::numeric::ublas::vector<T, ArrT> const> {
    typedef boost::numeric::ublas::vector<T, ArrT> vector_type;
    typedef T const* pointer; 
    static pointer storage (vector_type const& v) {
      return storage_traits<ArrT const>::storage (v.data()); 
    }
    static std::ptrdiff_t stride (vector_type const& v) { return 1; }
  }; 

  // ublas::vector_range<>
  template <typename Vct>
  struct storage_traits<boost::numeric::ublas::vector_range<Vct> > {
    typedef boost::numeric::ublas::vector_range<Vct> vector_type; 
    typedef typename storage_traits<Vct>::pointer pointer;
    static pointer storage (vector_type& vr) {
      pointer ptr = storage_traits<Vct>::storage (vr.data()); 
      ptr += vr.start() * storage_traits<Vct>::stride (vr.data());
      return ptr; 
    }
    static std::ptrdiff_t stride (vector_type& vr) {
      return storage_traits<Vct>::stride (vr.data()); 
    }
  }; 
  template <typename Vct>
  struct storage_traits<boost::numeric::ublas::vector_range<Vct> const> {
    typedef boost::numeric::ublas::vector_range<Vct> vector_type; 
    typedef typename storage_traits<Vct const>::pointer pointer; 
    static 
    pointer storage (vector_type const& vr) {
      pointer ptr = storage_traits<Vct const>::storage (vr.data()); 
      ptr += vr.start() * storage_traits<Vct const>::stride (vr.data());
      return ptr; 
    }
    static std::ptrdiff_t stride (vector_type const& vr) {
      return storage_traits<Vct const>::stride (vr.data()); 
    }
  }; 

  // ublas::vector_slice<>
  template <typename Vct>
  struct storage_traits<boost::numeric::ublas::vector_slice<Vct> > {
    typedef boost::numeric::ublas::vector_slice<Vct> vector_type; 
    typedef typename storage_traits<Vct>::pointer pointer; 
    static pointer storage (vector_type& vs) {
      pointer ptr = storage_traits<Vct>::storage (vs.data()); 
      ptr += vs.start() * storage_traits<Vct>::stride (vs.data());
      return ptr; 
    }
    static std::ptrdiff_t stride (vector_type& vs) {
      return vs.stride() * storage_traits<Vct>::stride (vs.data()); 
    }
  }; 
  template <typename Vct>
  struct storage_traits<boost::numeric::ublas::vector_slice<Vct> const> {
    typedef boost::numeric::ublas::vector_slice<Vct> vector_type; 
    typedef typename storage_traits<Vct const>::pointer pointer; 
    static pointer storage (vector_type const& vs) {
      pointer ptr = storage_traits<Vct const>::storage (vs.data()); 
      ptr += vs.start() * storage_traits<Vct const>::stride (vs.data());
      return ptr; 
    }
    static std::ptrdiff_t stride (vector_type const& vs) {
      return vs.stride() * storage_traits<Vct const>::stride (vs.data()); 
    }
  }; 

}}}  

#endif // BOOST_BLASBINDINGS_STORAGE_TRAITS_UBLAS_VECTOR_H
