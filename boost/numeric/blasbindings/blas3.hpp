#ifndef BOOST_BLASBINDINGS_BLAS3_HPP
#define BOOST_BLASBINDINGS_BLAS3_HPP

#include <boost/numeric/blasbindings/blaspp.hpp>
#include <boost/numeric/blasbindings/blas_traits.hpp>
#include <boost/numeric/blasbindings/transpose.hpp>

namespace boost { namespace numeric { namespace blasbindings {

  // C <- alpha * op (A) * op (B) + beta * C 
  // op (X) == X || X^T || X^H
  template < 
    typename matrix_type_a, typename matrix_type_b, typename matrix_type_c 
    >
  void gemm(const char TRANSA, const char TRANSB, 
	    const typename 
	      blas_matrix_traits<matrix_type_a>::value_type& alpha,
	    const matrix_type_a &a,
	    const matrix_type_b &b,
	    const typename 
	      blas_matrix_traits<matrix_type_c>::value_type &beta,
	    matrix_type_c &c
	    )
  {
    typedef blas_matrix_traits< const matrix_type_a > matraits ; 
    typedef blas_matrix_traits< const matrix_type_b > mbtraits ; 
    typedef blas_matrix_traits< matrix_type_c > mctraits ; 

    typedef typename mctraits::value_type  value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

    const int m = matraits::size1( a );
    const int n = mbtraits::size2( b );
    const int k = matraits::size2( a );
    assert( m == mctraits::size1( c ) ); // m
    assert( n == mctraits::size2( c ) ); // n
    assert( k == mbtraits::size1( c ) ); // k
    const int lda = m;
    const int ldb = k;
    const int ldc = m;

    const value_type *a_ptr = matraits::storage( a ) ;
    const value_type *b_ptr = mbtraits::storage( b ) ;
    value_type *c_ptr = mctraits::storage( c ) ;

    blas< value_type >::gemm( &TRANSA, &TRANSB, 
			      &m, &n, &k, 
			      (const bind_type*)&alpha, 
			      (const bind_type*)a_ptr, &lda, 
			      (const bind_type*)b_ptr, &ldb, 
			      (const bind_type*)&beta, 
			      (bind_type*)c_ptr, &ldc 
			      ) ;
  }


  // C <- alpha * A * B + beta * C 
  template < 
    typename matrix_type_a, typename matrix_type_b, typename matrix_type_c 
    >
  void gemm(const typename 
	      blas_matrix_traits<matrix_type_a>::value_type& alpha,
	    const matrix_type_a &a,
	    const matrix_type_b &b,
	    const typename 
	      blas_matrix_traits<matrix_type_c>::value_type &beta,
	    matrix_type_c &c
	    )
  {
    gemm( NO_TRANSPOSE, NO_TRANSPOSE, alpha, a, b, beta, c ) ;
  }


  // C <- A * B 
  template < 
    typename matrix_type_a, typename matrix_type_b, typename matrix_type_c 
    >
  void gemm(const matrix_type_a &a, const matrix_type_b &b, matrix_type_c &c)
  {
    typedef typename blas_matrix_traits<matrix_type_c>::value_type val_t; 
    gemm( NO_TRANSPOSE, NO_TRANSPOSE, (val_t) 1, a, b, (val_t) 0, c ) ;
  }

}}}

#endif // BOOST_BLASBINDINGS_BLAS3_HPP
