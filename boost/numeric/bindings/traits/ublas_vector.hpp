// (-*- C++ -*- header)  

/*
 * 
 * Copyright (c) Kresimir Fresl 2002 
 *
 * Permission to copy, modify, use and distribute this software 
 * for any non-commercial or commercial purpose is granted provided 
 * that this license appear on all copies of the software source code.
 *
 * Author assumes no responsibility whatsoever for its use and makes 
 * no guarantees about its quality, correctness or reliability.
 *
 * Author acknowledges the support of the Faculty of Civil Engineering, 
 * University of Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_VECTOR_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_VECTOR_H

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_vector_closure.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

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

  private:
    typedef typename vector_type::vector_closure_type closure_t; 
    typedef detail::closure_traits<closure_t> cl_traits; 

  public:
    static pointer storage (vector_type& vr) {
      typename cl_traits::type& vt = cl_traits::get (vr.data()); 
      pointer ptr = vector_traits<V>::storage (vt); 
      ptr += vr.start() * vector_traits<V>::stride (vt);
      return ptr; 
    }
    static int stride (vector_type& vr) {
      return vector_traits<V>::stride (cl_traits::get (vr.data())); 
    }
  }; 

  template <typename V>
  struct vector_traits<boost::numeric::ublas::vector_range<V> const> 
  : default_vector_traits<boost::numeric::ublas::vector_range<V> const> 
  {
    typedef boost::numeric::ublas::vector_range<V> vector_type; 
    typedef 
      typename default_vector_traits<vector_type const>::pointer pointer; 

  private:
    typedef vector_type const c_vector_type; 
    typedef typename c_vector_type::vector_closure_type closure_t; 
    typedef detail::closure_traits<closure_t const> cl_traits; 

  public:
    static pointer storage (vector_type const& vr) {
      typename cl_traits::type const& vt = cl_traits::get (vr.data()); 
      pointer ptr = vector_traits<V const>::storage (vt); 
      ptr += vr.start() * vector_traits<V const>::stride (vt);
      return ptr; 
    }
    static int stride (vector_type const& vr) {
      return vector_traits<V const>::stride (cl_traits::get (vr.data())); 
    }
  }; 

  // ublas::vector_slice<>
  template <typename V>
  struct vector_traits<boost::numeric::ublas::vector_slice<V> > 
  : default_vector_traits<boost::numeric::ublas::vector_slice<V> > 
  {
    typedef boost::numeric::ublas::vector_slice<V> vector_type; 
    typedef typename default_vector_traits<vector_type>::pointer pointer; 

  private:
    typedef typename vector_type::vector_closure_type closure_t; 
    typedef detail::closure_traits<closure_t> cl_traits; 

  public:
    static pointer storage (vector_type& vs) {
      typename cl_traits::type& vt = cl_traits::get (vs.data()); 
      pointer ptr = vector_traits<V>::storage (vt); 
      ptr += vs.start() * vector_traits<V>::stride (vt);
      return ptr; 
    }
    static int stride (vector_type& vs) {
      return vs.stride() 
	* vector_traits<V>::stride (cl_traits::get (vs.data())); 
    }
  }; 

  template <typename V>
  struct vector_traits<boost::numeric::ublas::vector_slice<V> const> 
  : default_vector_traits<boost::numeric::ublas::vector_slice<V> const> 
  {
    typedef boost::numeric::ublas::vector_slice<V> vector_type; 
    typedef 
      typename default_vector_traits<vector_type const>::pointer pointer; 

  private:
    typedef vector_type const c_vector_type; 
    typedef typename c_vector_type::vector_closure_type closure_t; 
    typedef detail::closure_traits<closure_t const> cl_traits; 

  public:
    static pointer storage (vector_type const& vs) {
      typename cl_traits::type const& vt = cl_traits::get (vs.data()); 
      pointer ptr = vector_traits<V const>::storage (vt); 
      ptr += vs.start() * vector_traits<V const>::stride (vt);
      return ptr; 
    }
    static int stride (vector_type const& vs) {
      return vs.stride() 
	* vector_traits<V const>::stride (cl_traits::get (vs.data())); 
    }
  }; 

}}}}  

#endif 
