#ifndef BOOST_BLASBINDINGS_BLAS3_HPP
#define BOOST_BLASBINDINGS_BLAS3_HPP

#include <boost/numeric/bindings/blas/blaspp.hpp>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/traits/transpose.hpp>

namespace boost { namespace numeric { namespace bindings { namespace blas {

  // C <- alpha * op (A) * op (B) + beta * C 
  // op (X) == X || X^T || X^H
  template < 
    typename matrix_type_a, typename matrix_type_b, typename matrix_type_c 
    >
  void gemm(const char TRANSA, const char TRANSB, 
	    const typename matrix_traits<matrix_type_a>::value_type& alpha,
	    const matrix_type_a &a,
	    const matrix_type_b &b,
	    const typename matrix_traits<matrix_type_c>::value_type &beta,
	    matrix_type_c &c
	    )
  {
    typedef matrix_traits< const matrix_type_a > matraits ; 
    typedef matrix_traits< const matrix_type_b > mbtraits ; 
    typedef matrix_traits< matrix_type_c > mctraits ; 

    typedef typename mctraits::value_type  value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

    const int m = TRANSA == NO_TRANSPOSE 
      ? matraits::size1( a )
      : matraits::size2( a ) ;
    const int n = TRANSB == NO_TRANSPOSE 
      ? mbtraits::size2( b )
      : mbtraits::size1( b );
    const int k = TRANSA == NO_TRANSPOSE 
      ? matraits::size2( a )
      : matraits::size1( a ) ;
    const int k1 = TRANSB == NO_TRANSPOSE 
      ? mbtraits::size1( b )
      : mbtraits::size2( b );
    assert( m == mctraits::size1( c ) ); 
    assert( n == mctraits::size2( c ) ); 
    assert( k == k1 ); 
    const int lda = matraits::leading_dimension( a );
    const int ldb = mbtraits::leading_dimension( b );
    const int ldc = mctraits::leading_dimension( c );

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
  void gemm(const typename matrix_traits<matrix_type_a>::value_type& alpha,
	    const matrix_type_a &a,
	    const matrix_type_b &b,
	    const typename matrix_traits<matrix_type_c>::value_type &beta,
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
    typedef typename matrix_traits<matrix_type_c>::value_type val_t; 
    gemm( NO_TRANSPOSE, NO_TRANSPOSE, (val_t) 1, a, b, (val_t) 0, c ) ;
  }

}}}}

#endif // BOOST_BLASBINDINGS_BLAS3_HPP
