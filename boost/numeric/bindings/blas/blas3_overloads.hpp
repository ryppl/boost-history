//  Copyright (C) Toon Knapen 2003
//
//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_NUMERIC_BINDINGS_BLAS_BLAS3_OVERLOADS_HPP
#define BOOST_NUMERIC_BINDINGS_BLAS_BLAS3_OVERLOADS_HPP

#include <boost/numeric/bindings/blas/blas.h>
#include <boost/numeric/bindings/traits/type.hpp>

namespace boost { namespace numeric { namespace bindings { namespace blas { namespace detail {

  using namespace boost::numeric::bindings::traits ;

  void gemm( char TRANSA, char TRANSB, const int& m, const int& n, const int& k, const float    & alpha, const float    * a_ptr, const int& lda, const float    * b_ptr, const int& ldb, const float    & beta, float    * c_ptr, const int& ldc ) { BLAS_SGEMM( &TRANSA, &TRANSB, &m, &n, &k,          ( &alpha ),          ( a_ptr ), &lda,          ( b_ptr ), &ldb,          ( &beta ),          ( c_ptr ), &ldc ) ; }
  void gemm( char TRANSA, char TRANSB, const int& m, const int& n, const int& k, const double   & alpha, const double   * a_ptr, const int& lda, const double   * b_ptr, const int& ldb, const double   & beta, double   * c_ptr, const int& ldc ) { BLAS_DGEMM( &TRANSA, &TRANSB, &m, &n, &k,          ( &alpha ),          ( a_ptr ), &lda,          ( b_ptr ), &ldb,          ( &beta ),          ( c_ptr ), &ldc ) ; }
  void gemm( char TRANSA, char TRANSB, const int& m, const int& n, const int& k, const complex_f& alpha, const complex_f* a_ptr, const int& lda, const complex_f* b_ptr, const int& ldb, const complex_f& beta, complex_f* c_ptr, const int& ldc ) { BLAS_CGEMM( &TRANSA, &TRANSB, &m, &n, &k, real_cast( &alpha ), real_cast( a_ptr ), &lda, real_cast( b_ptr ), &ldb, real_cast( &beta ), real_cast( c_ptr ), &ldc ) ; }
  void gemm( char TRANSA, char TRANSB, const int& m, const int& n, const int& k, const complex_d& alpha, const complex_d* a_ptr, const int& lda, const complex_d* b_ptr, const int& ldb, const complex_d& beta, complex_d* c_ptr, const int& ldc ) { BLAS_ZGEMM( &TRANSA, &TRANSB, &m, &n, &k, real_cast( &alpha ), real_cast( a_ptr ), &lda, real_cast( b_ptr ), &ldb, real_cast( &beta ), real_cast( c_ptr ), &ldc ) ; }

}}}}}

#endif // BOOST_NUMERIC_BINDINGS_BLAS_BLAS3_OVERLOADS_HPP

