#ifndef BOOST_BLASBINDINGS_BLAS2_HPP
#define BOOST_BLASBINDINGS_BLAS2_HPP

#include <cassert> 

#include <boost/numeric/blasbindings/blaspp.hpp>
#include <boost/numeric/blasbindings/traits.hpp>
#include <boost/numeric/blasbindings/transpose.hpp>

namespace boost { namespace numeric { namespace blasbindings {

  // y <- alpha * op (A) * x + beta * y
  // op (A) == A || A^T || A^H
  template < 
    typename matrix_type, typename vector_type_x, typename vector_type_y 
    >
  void gemv(const char TRANS, 
	    const typename matrix_traits<matrix_type>::value_type &alpha, 
	    const matrix_type &a, 
	    const vector_type_x &x, 
	    const typename vector_traits<vector_type_y>::value_type &beta,
	    vector_type_y &y
	    )
  {
    typedef matrix_traits< const matrix_type >                     mtraits ; 
    typedef vector_traits< const vector_type_x >                   xvtraits ; 
    typedef vector_traits< vector_type_y >                         yvtraits ; 
    typedef typename mtraits::value_type                    value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

    const int m = yvtraits::size( y ) ;
    const int n = xvtraits::size( x ) ;
    assert( m == static_cast< int >( mtraits::size1( a ) ) ) ; 
    assert( n == static_cast< int >( mtraits::size2( a ) ) ) ; 
    const int lda = m ; 
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
  void gemv(const typename matrix_traits<matrix_type>::value_type &alpha, 
	    const matrix_type &a, 
	    const vector_type_x &x, 
	    const typename vector_traits<vector_type_y>::value_type &beta,
	    vector_type_y &y
	    )
  {
    gemv( NO_TRANSPOSE, alpha, a, x, beta, y ); 
  }


  // y <- A x
  template < 
    typename matrix_type, typename vector_type_x, typename vector_type_y 
    >
  inline 
  void gemv(const matrix_type &a, const vector_type_x &x, vector_type_y &y)
  {
    typedef typename matrix_traits<matrix_type>::value_type val_t; 
    gemv( NO_TRANSPOSE, (val_t) 1, a, x, (val_t) 0, y );
  }

}}}

#endif // BOOST_BLASBINDINGS_BLAS2_HPP
