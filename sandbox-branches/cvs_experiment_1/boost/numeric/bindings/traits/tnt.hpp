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
 * Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_TNT_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_TNT_H

#include <boost/numeric/bindings/traits/traits.hpp>
#include <tnt/tnt_array1d.h>
#include <tnt/tnt_fortran_array1d.h>
#include <tnt/tnt_array2d.h>
#include <tnt/tnt_fortran_array2d.h>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // TNT::Array1D<>
  template <typename T>
  struct vector_traits<TNT::Array1D<T> > 
  {
    typedef T value_type; 
    typedef T* pointer; 
    static pointer storage (TNT::Array1D<T>& v) { return &v[0]; }
    static int size (TNT::Array1D<T>& v) { return v.dim(); } 
    static int stride (TNT::Array1D<T>& v) { return 1; } 
  }; 

  template <typename T>
  struct vector_traits<TNT::Array1D<T> const> 
  {
    typedef T value_type; 
    typedef T const* pointer; 
    static pointer storage (TNT::Array1D<T> const& v) { return &v[0]; }
    static int size (TNT::Array1D<T> const& v) { return v.dim(); } 
    static int stride (TNT::Array1D<T> const& v) { return 1; } 
  }; 

  // TNT::Fortran_Array1D<>
  template <typename T>
  struct vector_traits<TNT::Fortran_Array1D<T> > 
  {
    typedef T value_type; 
    typedef T* pointer; 
    static pointer storage (TNT::Fortran_Array1D<T>& v) { return &v(1); }
    static int size (TNT::Fortran_Array1D<T>& v) { return v.dim(); } 
    static int stride (TNT::Fortran_Array1D<T>& v) { return 1; } 
  }; 

  template <typename T>
  struct vector_traits<TNT::Fortran_Array1D<T> const> 
  {
    typedef T value_type; 
    typedef T const* pointer; 
    static pointer storage (TNT::Fortran_Array1D<T> const& v) { return &v(1); }
    static int size (TNT::Fortran_Array1D<T> const& v) { return v.dim(); } 
    static int stride (TNT::Fortran_Array1D<T> const& v) { return 1; } 
  }; 


  // TNT::Array2D<>
  template <typename T>
  struct matrix_traits<TNT::Array2D<T> > 
  {
    typedef T value_type; 
    typedef T* pointer; 
    static pointer storage (TNT::Array2D<T>& m) { return m[0]; }
    static int size1 (TNT::Array2D<T>& m) { return m.dim1(); } 
    static int size2 (TNT::Array2D<T>& m) { return m.dim2(); } 
    static int leading_dimension (TNT::Array2D<T>& m) { return m.dim2(); } 
    typedef row_major_t ordering_type; 
  }; 

  template <typename T>
  struct matrix_traits<TNT::Array2D<T> const> 
  {
    typedef T value_type; 
    typedef T const* pointer; 
    static pointer storage (TNT::Array2D<T> const& m) { return m[0]; }
    static int size1 (TNT::Array2D<T> const& m) { return m.dim1(); } 
    static int size2 (TNT::Array2D<T> const& m) { return m.dim2(); } 
    static int leading_dimension (TNT::Array2D<T> const& m) { 
      return m.dim2(); 
    } 
    typedef row_major_t ordering_type; 
  }; 

  // TNT::Fortran_Array2D<>
  template <typename T>
  struct matrix_traits<TNT::Fortran_Array2D<T> > 
  {
    typedef T value_type; 
    typedef T* pointer; 
    static pointer storage (TNT::Fortran_Array2D<T>& m) { return &m(1, 1); }
    static int size1 (TNT::Fortran_Array2D<T>& m) { return m.dim1(); } 
    static int size2 (TNT::Fortran_Array2D<T>& m) { return m.dim2(); } 
    static int leading_dimension (TNT::Fortran_Array2D<T>& m) { 
      return m.dim1(); 
    } 
    typedef column_major_t ordering_type; 
  }; 

  template <typename T>
  struct matrix_traits<TNT::Fortran_Array2D<T> const> 
  {
    typedef T value_type; 
    typedef T const* pointer; 
    static pointer storage (TNT::Fortran_Array2D<T> const& m) { 
      return &m(1, 1); 
    }
    static int size1 (TNT::Fortran_Array2D<T> const& m) { return m.dim1(); } 
    static int size2 (TNT::Fortran_Array2D<T> const& m) { return m.dim2(); } 
    static int leading_dimension (TNT::Fortran_Array2D<T> const& m) { 
      return m.dim1(); 
    } 
    typedef column_major_t ordering_type; 
  }; 

}}}}  

#endif 
