#ifndef boost_ublas_blas3_hpp
#define boost_ublas_blas3_hpp

#include <math/blaspp.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/type_traits.hpp>


namespace boost { namespace numeric { namespace ublas {

  template < typename matrix_type_a, typename matrix_type_b, typename matrix_type_c >
  void gemm(const typename matrix_type_a::value_type& alpha, const matrix_type_a &a, const matrix_type_b &b, const typename matrix_type_c::value_type &beta, matrix_type_c &c)
  {
    typedef typename matrix_type_c::value_type  value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

    assert( a.size1() == c.size1() ); // m
    assert( b.size2() == c.size2() ); // n
    assert( a.size2() == b.size1() ); // k
    const int m = a.size1();
    const int n = b.size2();
    const int k = a.size2();
    const int lda = m;
    const int ldb = k;
    const int ldc = m;
    const value_type *a_ptr = array_ptr( a ) ;
    const value_type *b_ptr = array_ptr( b ) ;
    value_type *c_ptr       = array_ptr( c ) ;
    blas< value_type >::gemm( &NO_TRANSPOSE, &NO_TRANSPOSE, &m, &n, &k, (const bind_type*)&alpha, (const bind_type*)a_ptr, &lda, (const bind_type*)b_ptr, &ldb, (const bind_type*)&beta, (bind_type*)c_ptr, &ldc );
  }

}}}

#endif // boost_ublas_blas3_hpp
