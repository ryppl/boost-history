//  Copyright (C) Toon Knapen 2003
//
//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_NUMERIC_BINDINGS_BLAS_BLAS1_OVERLOADS_HPP
#define BOOST_NUMERIC_BINDINGS_BLAS_BLAS1_OVERLOADS_HPP

#include <boost/numeric/bindings/traits/type.hpp>

namespace boost { namespace numeric { namespace bindings { namespace blas { namespace detail {

  void scal(const int& n, const float&     alpha, float*     x, const int& incx) { BLAS_SSCAL( &n,            &alpha,              x  , &incx ) ; } 
  void scal(const int& n, const double&    alpha, double*    x, const int& incx) { BLAS_DSCAL( &n,            &alpha,              x  , &incx ) ; }
  void scal(const int& n, const complex_f& alpha, complex_f* x, const int& incx) { BLAS_CSCAL( &n, real_cast( &alpha ), real_cast( x ), &incx ) ; }
  void scal(const int& n, const complex_d& alpha, complex_d* x, const int& incx) { BLAS_ZSCAL( &n, real_cast( &alpha ), real_cast( x ), &incx ) ; }

}}}}}
#endif // BOOST_NUMERIC_BINDINGS_BLAS_BLAS1_OVERLOADS_HPP

