//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright Toon Knapen and Kresimir Fresl

#ifndef BOOST_BINDINGS_BLAS_BLAS2_HPP
#define BOOST_BINDINGS_BLAS_BLAS2_HPP

#include <cassert> 

#include <boost/numeric/bindings/blas/blaspp.hpp>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/traits/transpose.hpp>
#include <boost/numeric/ublas/traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

namespace boost { namespace numeric { namespace bindings { namespace blas {

  // y <- alpha * op (A) * x + beta * y
  // op (A) == A || A^T || A^H
  template < 
    typename matrix_type, typename vector_type_x, typename vector_type_y 
    >
  void gemv(const char TRANS, 
	    const typename traits::matrix_traits<matrix_type>::value_type &alpha, 
	    const matrix_type &a, 
	    const vector_type_x &x, 
	    const typename traits::vector_traits<vector_type_y>::value_type &beta,
	    vector_type_y &y
	    )
  {
    // precondition: matrix_type must be dense or dense_proxy
    BOOST_STATIC_ASSERT( ( boost::is_same< typename matrix_type::storage_category, 
                                           boost::numeric::ublas::dense_tag 
                                         >::value ) ||
                         ( boost::is_same< typename matrix_type::storage_category, 
                                           boost::numeric::ublas::dense_proxy_tag 
                                         >::value ) 
                        ) ;

    typedef traits::matrix_traits< const matrix_type > mtraits ; 
    typedef traits::vector_traits< const vector_type_x > xvtraits ; 
    typedef traits::vector_traits< vector_type_y > yvtraits ; 
    typedef typename mtraits::value_type value_type ;
    typedef typename traits::value_traits< value_type >::value_type bind_type ;

    const int m = mtraits::size1( a ) ;
    const int n = mtraits::size2( a ) ;
    assert ( xvtraits::size( x ) >= (TRANS == traits::NO_TRANSPOSE ? n : m) ) ; 
    assert ( yvtraits::size( y ) >= (TRANS == traits::NO_TRANSPOSE ? m : n) ) ; 
    const int lda = mtraits::leading_dimension( a ) ; 
    const int stride_x = xvtraits::stride( x ) ;
    const int stride_y = yvtraits::stride( y ) ;

    const value_type *a_ptr = mtraits::storage( a ) ;
    const value_type *x_ptr = xvtraits::storage( x ) ;
    value_type *y_ptr = yvtraits::storage( y ) ;

    blas< value_type >::gemv( &TRANS, &m, &n, 
			      (const bind_type*)&alpha, 
			      (const bind_type*)a_ptr, &lda, 
			      (const bind_type*)x_ptr, &stride_x, 
			      (const bind_type*)&beta, 
			      (bind_type*)y_ptr, &stride_y 
			      );
  }


  // y <- alpha * A * x + beta * y 
  template < 
    typename matrix_type, typename vector_type_x, typename vector_type_y 
    >
  inline 
  void gemv(const typename traits::matrix_traits<matrix_type>::value_type &alpha, 
	    const matrix_type &a, 
	    const vector_type_x &x, 
	    const typename traits::vector_traits<vector_type_y>::value_type &beta,
	    vector_type_y &y
	    )
  {
    gemv( traits::NO_TRANSPOSE, alpha, a, x, beta, y ); 
  }


  // y <- A * x
  template < 
    typename matrix_type, typename vector_type_x, typename vector_type_y 
    >
  inline 
  void gemv(const matrix_type &a, const vector_type_x &x, vector_type_y &y)
  {
    typedef typename traits::matrix_traits<matrix_type>::value_type val_t; 
    gemv( traits::NO_TRANSPOSE, (val_t) 1, a, x, (val_t) 0, y );
  }

}}}}

#endif // BOOST_BINDINGS_BLAS_BLAS2_HPP
