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
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/bindings/traits/config.hpp>
#include <boost/numeric/bindings/traits/vector_traits.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_vector_closure.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

#if ! defined(BOOST_NO_CV_TEMPLATE_TEMPLATES)

  // ublas::vector<>
  template <typename T, typename ArrT>
  struct vector_traits<boost::numeric::ublas::vector<T, ArrT> > 
  : default_vector_traits<boost::numeric::ublas::vector<T, ArrT>, T > 
  {
    typedef boost::numeric::ublas::vector<T, ArrT> vector_type;
    static T* storage (vector_type& v) {
      return vector_traits<ArrT>::storage (v.data()); 
    }
  }; 

  template <typename T, typename ArrT>
  struct vector_traits<boost::numeric::ublas::vector<T, ArrT> const> 
  : default_vector_traits<boost::numeric::ublas::vector<T, ArrT> const, T> 
  {
    typedef boost::numeric::ublas::vector<T, ArrT> vector_type;
    static T const* storage (vector_type const& v) {
      return vector_traits<ArrT const>::storage (v.data()); 
    }
  }; 
#endif // BOOST_NO_CV_TEMPLATE_TEMPLATES

  // ublas::vector_range<>
  template <typename V>
  struct vector_traits<boost::numeric::ublas::vector_range<V> > 
  : default_vector_traits<boost::numeric::ublas::vector_range<V>, typename V::value_type > 
  {
    typedef typename V::value_type value_type ;
    typedef boost::numeric::ublas::vector_range<V> vector_type; 
    typedef typename default_vector_traits<vector_type,value_type>::pointer pointer; 

  private:
    typedef typename vector_type::vector_closure_type closure_t; 
    typedef detail::closure_traits<closure_t> cl_traits; 

  public:
#ifndef BOOST_NO_CV_TEMPLATE_TEMPLATES
    static pointer storage (vector_type& vr) {
      typename cl_traits::type& vt = cl_traits::get (vr.data()); 
      pointer ptr = vector_traits<V>::storage (vt); 
      ptr += vr.start() * vector_traits<V>::stride (vt);
      return ptr; 
    }
#endif // BOOST_NO_CV_TEMPLATE_TEMPLATES
    static int stride (vector_type& vr) {
      return vector_traits<V>::stride (cl_traits::get (vr.data())); 
    }
  }; 

#ifndef BOOST_NO_CV_TEMPLATE_TEMPLATES

  template <typename V>
  struct vector_traits<boost::numeric::ublas::vector_range<V> const> 
  : default_vector_traits<boost::numeric::ublas::vector_range<V> const, typename V::value_type> 
  {
    typedef typename V::value_type value_type ;
    typedef boost::numeric::ublas::vector_range<V> vector_type; 
    typedef typename default_vector_traits<vector_type const,value_type>::pointer pointer; 

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
#endif // BOOST_NO_CV_TEMPLATE_TEMPLATES

  // ublas::vector_slice<>
  template <typename V>
  struct vector_traits<boost::numeric::ublas::vector_slice<V> > 
  : default_vector_traits<boost::numeric::ublas::vector_slice<V>, typename V::value_type > 
  {
    typedef typename V::value_type value_type ;
    typedef boost::numeric::ublas::vector_slice<V> vector_type; 
    typedef typename default_vector_traits<vector_type,value_type>::pointer pointer; 

  private:
    typedef typename vector_type::vector_closure_type closure_t; 
    typedef detail::closure_traits<closure_t> cl_traits; 

  public:
#ifndef BOOST_NO_CV_TEMPLATE_TEMPLATES
    static pointer storage (vector_type& vs) {
      typename cl_traits::type& vt = cl_traits::get (vs.data()); 
      pointer ptr = vector_traits<V>::storage (vt); 
      ptr += vs.start() * vector_traits<V>::stride (vt);
      return ptr; 
    }
#endif // BOOST_NO_CV_TEMPLATE_TEMPLATES
    static int stride (vector_type& vs) {
      return vs.stride() 
	* vector_traits<V>::stride (cl_traits::get (vs.data())); 
    }
  }; 

#ifndef BOOST_NO_CV_TEMPLATE_TEMPLATES
  template <typename V>
  struct vector_traits<boost::numeric::ublas::vector_slice<V> const> 
  : default_vector_traits<boost::numeric::ublas::vector_slice<V> const, typename V::value_type> 
  {
    typedef typename V::value_type value_type ;
    typedef boost::numeric::ublas::vector_slice<V> vector_type; 
    typedef typename default_vector_traits<vector_type const,value_type>::pointer pointer; 

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
#endif // BOOST_NO_CV_TEMPLATE_TEMPLATES

#if 0 // need to rewrite actually
  template < typename T, typename A >
  inline 
  T* vector_storage(boost::numeric::ublas::vector< T, A >& v) {
    return v.data().begin() ;
  }

  template < typename T, typename A >
  inline 
  const T* vector_storage(const boost::numeric::ublas::vector< T, A >& v) {
    return v.data().begin() ;
  }

  // VisualAge can't handle return type V::value_type if only container is taken as template argument
  // template < typename V > const typename V::value_type* vector_storage(const boost::numeric::ublas::vector_range< V >& v) ;
  //
  template < typename T, typename A >
  T* vector_storage(boost::numeric::ublas::vector_range< boost::numeric::ublas::vector< T, A > >& v) {
    return vector_storage( v.data().expression() ) + v.start() * vector_stride( v.data().expression() ) ;
  }

  template < typename T, typename A >
  inline 
  const T* vector_storage(const boost::numeric::ublas::vector_range< boost::numeric::ublas::vector< T, A > >& v) {
    return vector_storage( v.data().expression() ) + v.start() * vector_stride( v.data().expression() ) ;
  }

  template < typename T, typename A >
  inline 
  const T* vector_storage(const boost::numeric::ublas::vector_range< const boost::numeric::ublas::vector< T, A > >& v) {
    return vector_storage( v.data().expression() ) + v.start() * vector_stride( v.data().expression() ) ;
  }

  // VisualAge can't handle return type V::value_type if only container is taken as template argument
  // template < typename V > const typename V::value_type* vector_storage(const boost::numeric::ublas::vector_slice< V >& v) ;
  //
  template < typename T, typename A >
  T* vector_storage(boost::numeric::ublas::vector_slice< boost::numeric::ublas::vector< T, A > >& v) {
    return vector_storage( v.data().expression() ) + v.start() * vector_stride( v.data().expression()  ) ;
  }

  template < typename T, typename A >
  inline 
  const T* vector_storage(const boost::numeric::ublas::vector_slice< boost::numeric::ublas::vector< T, A > >& v) {
    return vector_storage( v.data().expression() ) + v.start() * vector_stride( v.data().expression() ) ;
  }

  template < typename T, typename A >
  inline 
  const T* vector_storage(const boost::numeric::ublas::vector_slice< const boost::numeric::ublas::vector< T, A > >& v) {
    return vector_storage( v.data().expression() ) + v.start() * vector_stride( v.data().expression() ) ;
  }

  template < typename V >
  inline
  int vector_size( const V &v ) {
    return v.size() ;
  }
  
    template < typename V >
  inline
  int vector_size( const boost::numeric::ublas::vector_reference<V> &v ) {
    return size( v ) ;
  }
  
  template < typename V >
  inline
  int vector_stride( const boost::numeric::ublas::vector_range<V> &v ) {
    return stride( v.data() ) ;
  }
  template < typename V >
  inline
  int vector_stride( const boost::numeric::ublas::vector_slice<V> &v ) {
    return v.stride() * stride( v.data() ) ;
  }

#endif

}}}}  

#endif // BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#endif // BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_VECTOR_H
