//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright Toon Knapen and Kresimir Fresl

#ifndef BOOST_BINDINGS_BLAS_BLAS3_HPP
#define BOOST_BINDINGS_BLAS_BLAS3_HPP

#include <boost/numeric/bindings/blas/blas3_overloads.hpp>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/traits/transpose.hpp>

namespace boost { namespace numeric { namespace bindings { namespace blas {

  // C <- alpha * op (A) * op (B) + beta * C 
  // op (X) == X || X^T || X^H
  template < 
    typename matrix_type_a, typename matrix_type_b, typename matrix_type_c 
    >
  void gemm(const char TRANSA, const char TRANSB, 
	    const typename traits::matrix_traits<matrix_type_a>::value_type& alpha,
	    const matrix_type_a &a,
	    const matrix_type_b &b,
	    const typename traits::matrix_traits<matrix_type_c>::value_type &beta,
	    matrix_type_c &c
	    )
  {
    typedef traits::matrix_traits< const matrix_type_a > matraits ; 
    typedef traits::matrix_traits< const matrix_type_b > mbtraits ; 
    typedef traits::matrix_traits< matrix_type_c > mctraits ; 
    typedef typename matraits::value_type value_type ;

    const int m = TRANSA == traits::NO_TRANSPOSE ? matraits::size1( a ) : matraits::size2( a ) ;
    const int n = TRANSB == traits::NO_TRANSPOSE ? mbtraits::size2( b ) : mbtraits::size1( b );
    const int k = TRANSA == traits::NO_TRANSPOSE ? matraits::size2( a ) : matraits::size1( a ) ;
    assert( k ==  ( TRANSB == traits::NO_TRANSPOSE ? mbtraits::size1( b ) : mbtraits::size2( b ) ) ) ;
    assert( m == mctraits::size1( c ) ); 
    assert( n == mctraits::size2( c ) ); 
    const int lda = traits::leading_dimension( a );
    const int ldb = traits::leading_dimension( b );
    const int ldc = traits::leading_dimension( c );

    const value_type *a_ptr = traits::matrix_storage( a ) ;
    const value_type *b_ptr = traits::matrix_storage( b ) ;
    value_type *c_ptr = traits::matrix_storage( c ) ;

    detail::gemm( TRANSA, TRANSB, m, n, k, alpha, a_ptr, lda, b_ptr, ldb, beta, c_ptr, ldc ) ;
  }


  // C <- alpha * A * B + beta * C 
  template < 
    typename matrix_type_a, typename matrix_type_b, typename matrix_type_c 
    >
  void gemm(const typename traits::matrix_traits<matrix_type_a>::value_type& alpha,
	    const matrix_type_a &a,
	    const matrix_type_b &b,
	    const typename traits::matrix_traits<matrix_type_c>::value_type &beta,
	    matrix_type_c &c
	    )
  {
    gemm( traits::NO_TRANSPOSE, traits::NO_TRANSPOSE, alpha, a, b, beta, c ) ;
  }


  // C <- A * B 
  template < 
    typename matrix_type_a, typename matrix_type_b, typename matrix_type_c 
    >
  void gemm(const matrix_type_a &a, const matrix_type_b &b, matrix_type_c &c)
  {
    typedef typename traits::matrix_traits<matrix_type_c>::value_type val_t; 
    gemm( traits::NO_TRANSPOSE, traits::NO_TRANSPOSE, (val_t) 1, a, b, (val_t) 0, c ) ;
  }

}}}}

#endif // BOOST_BINDINGS_BLAS_BLAS3_HPP
