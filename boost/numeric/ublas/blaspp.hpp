//
// Copyright (C) 2000,2001,2002 Si-Lab b.v.b.a.
// 
// License is hereby granted to use and modify this software 
// as long as this copyright notice is retained and modifications
// are clearly marked.
// This License does not permit you to redistribute this software.
// 
//
#ifndef blaspp_hpp
#define blaspp_hpp

#ifndef __cplusplus
#error "include directly blas_h"
#endif

#include <boost/numeric/ublas/blas.h>
#include <boost/numeric/ublas/valuetraits.hpp>

  typedef std::complex< float >  float_complex ;
  typedef std::complex< double > double_complex ;

  template < typename T >
  struct blas
  {
    typedef typename value_traits<T>::value_type bind_type ;

    // level 1 types
    typedef void  (*scal_type)(const int *n, const bind_type *alpha, bind_type *x, const int *incx) ;
    typedef void  (*axpy_type)(const int *n, const bind_type *alpha, const bind_type *x, const int *incx, bind_type *y, const int *incy) ;
    typedef T     (*dot_type) (const int *n, const bind_type *x, const int *incx,  const bind_type *y, const int *incy) ;
    typedef void  (*dotu_type)(bind_type* ret, const int *n, const bind_type *x, const int *incx,  const bind_type *y, const int *incy) ;
    typedef void  (*dotc_type)(bind_type* ret, const int *n, const bind_type *x, const int *incx,  const bind_type *y, const int *incy) ;
    typedef void  (*nrm2_type)(bind_type* ret, const int *n, const bind_type *x, const int *incx) ;
    typedef void  (*asum_type)(bind_type* ret, const int *n, const bind_type *x, const int *incx) ;

    // level 2 types
    typedef void  (*gemv_type)(const char *trans, const int *m, const int *n, const bind_type *alpha, const bind_type *a, const int *lda, const bind_type *x, const int *incx, const bind_type *beta, bind_type *y, const int *incy) ;
    
    // level 3 types
    typedef void  (*gemm_type)(const char *transa, const char *transb, const int *m, const int *n, const int *k, const bind_type *alpha, const bind_type *a, const int *lda, const bind_type *b, const int *ldb, const bind_type *beta, bind_type *c, const int *ldc);

    // level 1 functions
    static scal_type scal ;
    static axpy_type axpy ;
    static dot_type  dot  ;
    static dotu_type dotu ;
    static dotc_type dotc ;
    static nrm2_type nrm2 ;
    static asum_type asum ;

    // level 2 functions
    static gemv_type gemv ;

    // level 3 functions
    static gemm_type gemm ;
  };

#endif // blaspp_hpp
