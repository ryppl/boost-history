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

#include <boost/numeric/bindings/traits/vector_traits.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_vector_closure.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // ublas::vector<>
  template <typename T, typename ArrT, typename V>
  struct vector_detail_traits< boost::numeric::ublas::vector<T, ArrT>, V > 
  : default_vector_traits< V, T > 
  {
    BOOST_STATIC_ASSERT( (::boost::is_same< boost::numeric::ublas::vector<T, ArrT>, typename boost::remove_const<V>::type >::value) );

    typedef V                                               vector_type;
    typedef typename default_vector_traits< V, T >::pointer pointer;
    typedef typename detail::generate_const<V,ArrT>::type   array_type ;

    static pointer storage (vector_type& v) {
      return vector_traits<array_type>::storage (v.data()); 
    }
  }; 

  // ublas::vector_range<>
  template <typename V, typename VR>
  struct vector_detail_traits<boost::numeric::ublas::vector_range<V>, VR > 
  : default_vector_traits< VR, typename V::value_type > 
  {
    BOOST_STATIC_ASSERT( (::boost::is_same< boost::numeric::ublas::vector_range<V>, typename boost::remove_const<VR>::type >::value) );

    typedef typename V::value_type                                 value_type ;
    typedef boost::numeric::ublas::vector_range<V>                 vector_type                ; 
    typedef VR                                                     c_vector_type; 
    typedef typename default_vector_traits<VR,value_type>::pointer pointer; 
    typedef typename detail::generate_const<VR,V>::type                    v_type; 

  private:
    typedef typename c_vector_type::vector_closure_type                          closure_t; 
    typedef detail::closure_traits<typename detail::generate_const<VR,closure_t>::type > cl_traits; 

  public:
    static pointer storage (c_vector_type& vr) {
      typename cl_traits::type& vt = cl_traits::get (vr.data()); 
      pointer ptr = vector_traits<v_type>::storage (vt); 
      ptr += vr.start() * vector_traits<v_type>::stride (vt);
      return ptr; 
    }
    static int stride (c_vector_type& vr) {
      return vector_traits<v_type>::stride (cl_traits::get (vr.data())); 
    }
  }; 


  // ublas::vector_slice<>
  template <typename V, typename VS>
  struct vector_detail_traits<boost::numeric::ublas::vector_slice<V>, VS > 
  : default_vector_traits< VS, typename V::value_type > 
  {
    BOOST_STATIC_ASSERT( (::boost::is_same< boost::numeric::ublas::vector_slice<V>, typename boost::remove_const<VS>::type >::value) );

    typedef typename V::value_type                                 value_type ;
    typedef boost::numeric::ublas::vector_slice<V>                 vector_type                ; 
    typedef VS                                                     c_vector_type; 
    typedef typename default_vector_traits<VS,value_type>::pointer pointer; 
    typedef typename detail::generate_const<VS,V>::type                    v_type; 

  private:
    typedef typename c_vector_type::vector_closure_type                          closure_t; 
    typedef detail::closure_traits<typename detail::generate_const<VS,closure_t>::type > cl_traits; 
    typedef typename V::value_type value_type ;

  public:
    static pointer storage (c_vector_type& vs) {
      typename cl_traits::type& vt = cl_traits::get (vs.data()); 
      pointer ptr = vector_traits<v_type>::storage (vt); 
      ptr += vs.start() * vector_traits<v_type>::stride (vt);
      return ptr; 
    }
    static int stride (c_vector_type& vs) {
      return vs.stride() 
	* vector_traits<v_type>::stride (cl_traits::get (vs.data())); 
    }
  }; 

}}}}  

#endif // BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS

#endif // BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_VECTOR_H
