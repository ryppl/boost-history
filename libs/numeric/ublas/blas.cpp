// Copyright blablabla

#include <boost/numeric/ublas/blas.hpp>

namespace boost { namespace numerics {
  blas_traits< double >::axpy_type  blas_traits< double >::axpy = BLAS_DAXPY;
  blas_traits< double >::dot_type   blas_traits< double >::dot  = BLAS_DDOT;
  blas_traits< double >::gemm_type  blas_traits< double >::gemm = BLAS_DGEMM;
  
  blas_traits< std::complex< double > >::axpy_type  blas_traits< std::complex< double > >::axpy = BLAS_ZAXPY;
  blas_traits< std::complex< double > >::dot_type   blas_traits< std::complex< double > >::dot  = BLAS_ZDOT;
  blas_traits< std::complex< double > >::gemm_type  blas_traits< std::complex< double > >::gemm = BLAS_ZGEMM;
}}
