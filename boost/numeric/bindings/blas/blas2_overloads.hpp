//  Copyright (C) Toon Knapen 2003
//
//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_NUMERIC_BINDINGS_BLAS_BLAS2_OVERLOADS_HPP
#define BOOST_NUMERIC_BINDINGS_BLAS_BLAS2_OVERLOADS_HPP

#include <boost/numeric/bindings/blas/blas.h>
#include <boost/numeric/bindings/traits/type.hpp>

namespace boost { namespace numeric { namespace bindings { namespace blas { namespace detail {

  using namespace boost::numeric::bindings::traits ;

  void gemv( char TRANS, const int& m, const int& n, const float    & alpha, const float    * a_ptr, const int& lda, const float    * x_ptr, const int& incx, const float    & beta, float    * y_ptr, const int& incy ) { BLAS_SGEMV( &TRANS, &m, &n,          ( &alpha ),          ( a_ptr ), &lda,          ( x_ptr ), &incx,          ( &beta ),          ( y_ptr ), &incy ) ; }
  void gemv( char TRANS, const int& m, const int& n, const double   & alpha, const double   * a_ptr, const int& lda, const double   * x_ptr, const int& incx, const double   & beta, double   * y_ptr, const int& incy ) { BLAS_DGEMV( &TRANS, &m, &n,          ( &alpha ),          ( a_ptr ), &lda,          ( x_ptr ), &incx,          ( &beta ),          ( y_ptr ), &incy ) ; }
  void gemv( char TRANS, const int& m, const int& n, const complex_f& alpha, const complex_f* a_ptr, const int& lda, const complex_f* x_ptr, const int& incx, const complex_f& beta, complex_f* y_ptr, const int& incy ) { BLAS_CGEMV( &TRANS, &m, &n, real_cast( &alpha ), real_cast( a_ptr ), &lda, real_cast( x_ptr ), &incx, real_cast( &beta ), real_cast( y_ptr ), &incy ) ; }
  void gemv( char TRANS, const int& m, const int& n, const complex_d& alpha, const complex_d* a_ptr, const int& lda, const complex_d* x_ptr, const int& incx, const complex_d& beta, complex_d* y_ptr, const int& incy ) { BLAS_ZGEMV( &TRANS, &m, &n, real_cast( &alpha ), real_cast( a_ptr ), &lda, real_cast( x_ptr ), &incx, real_cast( &beta ), real_cast( y_ptr ), &incy ) ; }

}}}}}

#endif // BOOST_NUMERIC_BINDINGS_BLAS_BLAS2_OVERLOADS_HPP

