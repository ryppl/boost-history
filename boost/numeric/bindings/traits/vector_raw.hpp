//
//  Copyright (c) 2002-2003
//  Toon Knapen, Kresimir Fresl, Joerg Walter
//
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.
//  It is provided "as is" without express or implied warranty.
//
//

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_VECTOR_RAW_HPP
#define BOOST_NUMERIC_BINDINGS_TRAITS_VECTOR_RAW_HPP

#include <boost/numeric/ublas/config.hpp> 
#include <boost/numeric/ublas/vector.hpp> 
#include <vector> 
#include <boost/numeric/bindings/traits/detail/array_impl.hpp> 

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // ublas::vector<> etc. 

  namespace ublas = boost::numeric::ublas; 

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
  template <typename T, typename A>
  BOOST_UBLAS_INLINE
  typename ublas::vector<T,A>::const_pointer 
  vector_storage (const ublas::vector<T,A> &v) {
    return &v.data().begin()[0];
  }
#endif
  // We need storage_const() mostly due to MSVC 6.0.
  // But how shall we write portable code otherwise?
  template <typename T, typename A>
  BOOST_UBLAS_INLINE
  typename ublas::vector<T,A>::const_pointer 
  vector_storage_const (const ublas::vector<T,A> &v) {
    return &v.data().begin()[0];
  }
  template <typename T, typename A>
  BOOST_UBLAS_INLINE
  typename ublas::vector<T,A>::pointer 
  vector_storage (ublas::vector<T,A> &v) {
    return &v.data().begin()[0];
  }

#if 0
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::const_pointer vector_storage (const V &v) {
    return &v.data().begin()[0];
  }
#endif
  // We need storage_const() mostly due to MSVC 6.0.
  // But how shall we write portable code otherwise?
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::const_pointer vector_storage_const (const V &v) {
    return &v.data().begin()[0];
  }
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::pointer vector_storage (V &v) {
    return &v.data().begin()[0];
  }
#endif 

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::const_pointer 
  vector_storage (const ublas::vector_reference<V> &v) {
    return vector_storage (v.expression());
  }
#endif
  // We need storage_const() mostly due to MSVC 6.0.
  // But how shall we write portable code otherwise?
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::const_pointer 
  vector_storage_const (const ublas::vector_reference<V> &v) {
    return vector_storage_const (v.expression());
  }
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::pointer vector_storage (ublas::vector_reference<V> &v) {
    return vector_storage (v.expression());
  }

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
  template <typename T, std::size_t N>
  BOOST_UBLAS_INLINE
  typename ublas::c_vector<T, N>::const_pointer 
  vector_storage (const ublas::c_vector<T, N> &v) {
    return v.data();
  }
#endif
#ifndef BOOST_MSVC
  // We need storage_const() mostly due to MSVC 6.0.
  // But how shall we write portable code otherwise?
  template <typename T, std::size_t N>
  BOOST_UBLAS_INLINE
  typename ublas::c_vector<T, N>::const_pointer 
  vector_storage_const (const ublas::c_vector<T, N> &v) {
    return v.data();
  }
  template <typename T, std::size_t N>
  BOOST_UBLAS_INLINE
  typename ublas::c_vector<T, N>::pointer 
  vector_storage (ublas::c_vector<T, N> &v) {
    return v.data();
  }
#endif

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::const_pointer vector_storage (const ublas::vector_range<V> &v) {
    return vector_storage (v.data()) + v.start();
  }
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::const_pointer vector_storage (const ublas::vector_slice<V> &v) {
    return vector_storage (v.data()) + v.start();
  }
#endif
  // We need storage_const() mostly due to MSVC 6.0.
  // But how shall we write portable code otherwise?
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::const_pointer 
  vector_storage_const (const ublas::vector_range<V> &v) {
    return vector_storage_const (v.data()) + v.start();
  }
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::const_pointer 
  vector_storage_const (const ublas::vector_slice<V> &v) {
    return vector_storage_const (v.data()) + v.start();
  }
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::pointer vector_storage (ublas::vector_range<V> &v) {
    return vector_storage (v.data()) + v.start();
  }
  template <typename V>
  BOOST_UBLAS_INLINE
  typename V::pointer vector_storage (ublas::vector_slice<V> &v) {
    return vector_storage (v.data()) + v.start();
  }


  template <typename V>
  BOOST_UBLAS_INLINE
  int vector_size (const V &v) {
    return v.size();
  }
  template <typename V>
  BOOST_UBLAS_INLINE
  int vector_size (const ublas::vector_reference<V> &v) {
    return vector_size (v.expression());
  }


  template <typename V>
  BOOST_UBLAS_INLINE
  int vector_stride (const V &v) {
    return 1;
  }
  template <typename V>
  BOOST_UBLAS_INLINE
  int vector_stride (const ublas::vector_range<V> &v) {
    return vector_stride (v.data());
  }
  template <typename V>
  BOOST_UBLAS_INLINE
  int vector_stride (const ublas::vector_slice<V> &v) {
    return v.stride() * vector_stride (v.data());
  }


  // std::vector<> 

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
  template <typename T, typename A>
  BOOST_UBLAS_INLINE
  typename std::vector<T, A>::const_pointer 
  vector_storage (const std::vector<T, A> &v) {
    return &v.front();
  }
#endif
  // We need storage_const() mostly due to MSVC 6.0.
  // But how shall we write portable code otherwise?
  template <typename T, typename A>
  BOOST_UBLAS_INLINE
  typename std::vector<T, A>::const_pointer 
  vector_storage_const (const std::vector<T, A> &v) {
    return &v.front();
  }
  template <typename T, typename A>
  BOOST_UBLAS_INLINE
  typename std::vector<T, A>::pointer vector_storage (std::vector<T, A> &v) {
    return &v.front();
  }


  // bindings::traits::detail::array<>

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
  template <typename T> 
  BOOST_UBLAS_INLINE
  const T* vector_storage (const detail::array<T> &a) { return a.storage(); }
#endif 
  // We need storage_const() mostly due to MSVC 6.0.
  // But how shall we write portable code otherwise?
  template <typename T> 
  BOOST_UBLAS_INLINE
  const T* vector_storage_const (const detail::array<T> &a) { 
    return a.storage(); 
  }
  template <typename T> 
  BOOST_UBLAS_INLINE
  T* vector_storage (detail::array<T> &a) { return a.storage(); }


}}}}

#endif
