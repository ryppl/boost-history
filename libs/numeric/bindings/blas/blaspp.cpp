// Copyright blablabla

#include <boost/numeric/bindings/blas/blaspp.hpp>

namespace boost { namespace numeric { namespace bindings { namespace blas {

template<> blas< float          >::scal_type  blas< float          >::scal = BLAS_SSCAL;
template<> blas< double         >::scal_type  blas< double         >::scal = BLAS_DSCAL;
template<> blas< traits::float_complex  >::scal_type  blas< traits::float_complex  >::scal = BLAS_CSCAL;
template<> blas< traits::double_complex >::scal_type  blas< traits::double_complex >::scal = BLAS_ZSCAL;

template<> blas< float          >::axpy_type  blas< float          >::axpy = BLAS_SAXPY;
template<> blas< double         >::axpy_type  blas< double         >::axpy = BLAS_DAXPY;
template<> blas< traits::float_complex  >::axpy_type  blas< traits::float_complex  >::axpy = BLAS_CAXPY;
template<> blas< traits::double_complex >::axpy_type  blas< traits::double_complex >::axpy = BLAS_ZAXPY;

template<> blas< float          >::dot_type   blas< float          >::dot  = BLAS_SDOT;
template<> blas< double         >::dot_type   blas< double         >::dot  = BLAS_DDOT;

template<> blas< traits::float_complex  >::dotu_type  blas< traits::float_complex  >::dotu = BLAS_CDOTU;
template<> blas< traits::double_complex >::dotu_type  blas< traits::double_complex >::dotu = BLAS_ZDOTU;

template<> blas< traits::float_complex  >::dotc_type  blas< traits::float_complex  >::dotc = BLAS_CDOTC;
template<> blas< traits::double_complex >::dotc_type  blas< traits::double_complex >::dotc = BLAS_ZDOTC;

template<> blas< float          >::nrm2_type   blas< float          >::nrm2  = BLAS_SNRM2;
template<> blas< double         >::nrm2_type   blas< double         >::nrm2  = BLAS_DNRM2;

template<> blas< float          >::asum_type   blas< float          >::asum  = BLAS_SASUM;
template<> blas< double         >::asum_type   blas< double         >::asum  = BLAS_DASUM;

template<> blas< float          >::gemv_type  blas< float          >::gemv = BLAS_SGEMV;
template<> blas< double         >::gemv_type  blas< double         >::gemv = BLAS_DGEMV;
template<> blas< traits::float_complex  >::gemv_type  blas< traits::float_complex  >::gemv = BLAS_CGEMV;
template<> blas< traits::double_complex >::gemv_type  blas< traits::double_complex >::gemv = BLAS_ZGEMV;

template<> blas< float          >::gemm_type  blas< float          >::gemm = BLAS_SGEMM;
template<> blas< double         >::gemm_type  blas< double         >::gemm = BLAS_DGEMM;
template<> blas< traits::float_complex  >::gemm_type  blas< traits::float_complex  >::gemm = BLAS_CGEMM;
template<> blas< traits::double_complex >::gemm_type  blas< traits::double_complex >::gemm = BLAS_ZGEMM;
  
}}}}
