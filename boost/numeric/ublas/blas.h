/*
 * Copyright (C) 2000,2001 Si-Lab b.v.b.a.
 * 
 * License is hereby granted to use and modify this software 
 * as long as this copyright notice is retained and modifications
 * are clearly marked.
 * This License does not permit you to redistribute this software.
 * 
 */
#ifndef blas_h
#define blas_h

#ifdef __cplusplus
#ifndef blaspp_hpp
#error "include blaspp.hpp instead"
#endif
#endif

#include <boost/numeric/ublas/fortran.h>
#include <complex>

//
// make sure to match the library symbols
// taking the right convention to link C with Fortran

#if defined( BIND_FORTRAN_LOWERCASE_UNDERSCORE )

#define BLAS_SSCAL sscal_
#define BLAS_DSCAL dscal_
#define BLAS_CSCAL cscal_
#define BLAS_ZSCAL zscal_

#define BLAS_SAXPY saxpy_
#define BLAS_DAXPY daxpy_
#define BLAS_CAXPY caxpy_
#define BLAS_ZAXPY zaxpy_

#define BLAS_SDOT  sdot_
#define BLAS_DDOT  ddot_

#define BLAS_CDOTU cdotu_
#define BLAS_ZDOTU zdotu_ 

#define BLAS_CDOTC cdotc_
#define BLAS_ZDOTC zdotc_

#define BLAS_SNRM2 snrm2_
#define BLAS_DNRM2 dnrm2_


#define BLAS_SGEMV sgemv_
#define BLAS_DGEMV dgemv_
#define BLAS_CGEMV cgemv_
#define BLAS_ZGEMV zgemv_

#define BLAS_SGER  sger_
#define BLAS_DGER  dger_

#define BLAS_CGERU cgeru_
#define BLAS_ZGERU zgeru_

#define BLAS_CGERC cgerc_
#define BLAS_ZGERC zgerc_



#define BLAS_SGEMM sgemm_
#define BLAS_DGEMM dgemm_
#define BLAS_CGEMM cgemm_
#define BLAS_ZGEMM zgemm_

#elif defined( BIND_FORTRAN_LOWERCASE )

#define BLAS_SSCAL sscal
#define BLAS_DSCAL dscal
#define BLAS_CSCAL cscal
#define BLAS_ZSCAL zscal

#define BLAS_SAXPY saxpy
#define BLAS_DAXPY daxpy
#define BLAS_CAXPY caxpy
#define BLAS_ZAXPY zaxpy

#define BLAS_SDOT  sdot
#define BLAS_DDOT  ddot

#define BLAS_CDOTU cdotu
#define BLAS_ZDOTU zdotu

#define BLAS_CDOTC cdotc
#define BLAS_ZDOTC zdotc

#define BLAS_SNRM2 snrm2
#define BLAS_DNRM2 dnrm2

#define BLAS_SGEMV sgemv
#define BLAS_DGEMV dgemv
#define BLAS_CGEMV cgemv
#define BLAS_ZGEMV zgemv

#define BLAS_SGER  sger
#define BLAS_DGER  dger

#define BLAS_CGERU cgeru
#define BLAS_ZGERU zgeru

#define BLAS_CGERC cgerc_
#define BLAS_ZGERC zgerc_

#define BLAS_SGEMM sgemm
#define BLAS_DGEMM dgemm
#define BLAS_CGEMM cgemm
#define BLAS_ZGEMM zgemm

#else
#error do not how to link with fortran
#endif
  
/*
 * const-correct prototypes for BLAS functions
 * 
 */

typedef float  fcomplex ;
typedef double dcomplex ;

extern "C"
{
  void   BLAS_SSCAL(const int *n, const float*    alpha, float*    x, const int* incx);
  void   BLAS_DSCAL(const int *n, const double*   alpha, double*   x, const int* incx);
  void   BLAS_CSCAL(const int *n, const fcomplex* alpha, fcomplex* x, const int* incx);
  void   BLAS_ZSCAL(const int *n, const dcomplex* alpha, dcomplex* x, const int* incx);

  void   BLAS_SAXPY(const int *n, const float*    alpha, const float*    x, const int* incx,  float*    y, const int* incy);
  void   BLAS_DAXPY(const int *n, const double*   alpha, const double*   x, const int* incx,  double*   y, const int* incy);
  void   BLAS_CAXPY(const int *n, const fcomplex* alpha, const fcomplex* x, const int* incx,  fcomplex* y, const int* incy);
  void   BLAS_ZAXPY(const int *n, const dcomplex* alpha, const dcomplex* x, const int* incx,  dcomplex* y, const int* incy);

  float  BLAS_SDOT (const int *n, const float  *x, const int *incx, const float  *y, const int *incy);
  double BLAS_DDOT (const int *n, const double *x, const int *incx, const double *y, const int *incy);

  void   BLAS_CDOTU(fcomplex* ret, const int *n, const fcomplex *x, const int *incx, const fcomplex *y, const int *incy);
  void   BLAS_ZDOTU(dcomplex* ret, const int *n, const dcomplex *x, const int *incx, const dcomplex *y, const int *incy); 
  
  void   BLAS_CDOTC(fcomplex* ret, const int *n, const fcomplex *x, const int *incx, const fcomplex *y, const int *incy);
  void   BLAS_ZDOTC(dcomplex* ret, const int *n, const dcomplex *x, const int *incx, const dcomplex *y, const int *incy);

  void   BLAS_SNRM2(float*  ret, const int *n, const float  *x, const int *incx);
  void   BLAS_DNRM2(double* ret, const int *n, const double *x, const int *incx);




  void   BLAS_SGEMV(const char *trans, const int *m, const int *n, const float    *alpha, const float    *a, const int *lda, const float    *x, const int *incx, const float    *beta, float    *y, const int *incy) ;
  void   BLAS_DGEMV(const char *trans, const int *m, const int *n, const double   *alpha, const double   *a, const int *lda, const double   *x, const int *incx, const double   *beta, double   *y, const int *incy) ;
  void   BLAS_CGEMV(const char *trans, const int *m, const int *n, const fcomplex *alpha, const fcomplex *a, const int *lda, const fcomplex *x, const int *incx, const fcomplex *beta, fcomplex *y, const int *incy) ;
  void   BLAS_ZGEMV(const char *trans, const int *m, const int *n, const dcomplex *alpha, const dcomplex *a, const int *lda, const dcomplex *x, const int *incx, const dcomplex *beta, dcomplex *y, const int *incy) ;

  void   BLAS_SGER (const int *m, const int *n, const float *alpha, const float *x, const int *incx, const float *y, const int *incy, float *a, const int *lda);
  void   BLAS_DGER (const int *m, const int *n, const double *alpha, const double *x, const int *incx, const double *y, const int *incy, double *a, const int *lda);

  void   BLAS_CGERU(const int *m, const int *n, const fcomplex *alpha, const fcomplex *x, const int *incx, const fcomplex *y, const int *incy, fcomplex *a, const int *lda);
  void   BLAS_ZGERU(const int *m, const int *n, const dcomplex *alpha, const dcomplex *x, const int *incx, const dcomplex *y, const int *incy, dcomplex *a, const int *lda);

  void   BLAS_CGERC(const int *m, const int *n, const fcomplex *alpha, const fcomplex *x, const int *incx, const fcomplex *y, const int *incy, fcomplex *a, const int *lda);
  void   BLAS_ZGERC(const int *m, const int *n, const dcomplex *alpha, const dcomplex *x, const int *incx, const dcomplex *y, const int *incy, dcomplex *a, const int *lda);




  void   BLAS_SGEMM(const char *transa, const char *transb, const int *m, const int *n, const int *k, const float    *alpha, const float    *a, const int *lda, const float    *b, const int *ldb, const float    *beta, float    *c, const int *ldc);
  void   BLAS_DGEMM(const char *transa, const char *transb, const int *m, const int *n, const int *k, const double   *alpha, const double   *a, const int *lda, const double   *b, const int *ldb, const double   *beta, double   *c, const int *ldc);
  void   BLAS_CGEMM(const char *transa, const char *transb, const int *m, const int *n, const int *k, const fcomplex *alpha, const fcomplex *a, const int *lda, const fcomplex *b, const int *ldb, const fcomplex *beta, fcomplex *c, const int *ldc);
  void   BLAS_ZGEMM(const char *transa, const char *transb, const int *m, const int *n, const int *k, const dcomplex *alpha, const dcomplex *a, const int *lda, const dcomplex *b, const int *ldb, const dcomplex *beta, dcomplex *c, const int *ldc);
}

#endif // blas_h
