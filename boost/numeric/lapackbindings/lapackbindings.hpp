#ifndef boost_numeric_lapackbindings_lapackbindings_hpp
#define boost_numeric_lapackbindings_lapackbindings_hpp

#include <boost/numeric/blasbindings/blas_traits.hpp>
#include <boost/numeric/blasbindings/transpose.hpp>
#include <boost/numeric/lapackbindings/lapack.hpp>

namespace boost { namespace numeric { namespace lapackbindings {

  template < typename matrix_type, typename IpivIterator >
  int getrf(matrix_type& a, IpivIterator begin_ipiv, IpivIterator end_ipiv)
  {
    typedef blasbindings::blas_matrix_traits< matrix_type > mtraits ; 
    typedef typename mtraits::value_type                    value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

    int m = mtraits::size1( a ) ;
    int n = mtraits::size2( a ) ;
    const value_type *a_ptr = mtraits::storage( a ) ;
    int lda = m ;
    assert( std::distance( begin_ipiv, end_ipiv ) >= std::min( m,  n ) );
    int info = 0;
    int& ipiv_ptr = *begin_ipiv ;
    lapack< value_type >::getrf( &m, &n, (bind_type*)a_ptr, &lda, &ipiv, &info );
    return info;
  }

  template <typename matrix_type, typename IntIterator >
  int getrs(char trans, matrix_type& a, IntIterator begin_ipiv, IntIterator end_ipiv, matrix_type& b)
  {
    typedef blasbindings::blas_matrix_traits< matrix_type > mtraits ; 
    typedef typename mtraits::value_type                    value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

    assert( trans == 'N' || trans == 'T' );
    int n = a.size1();
    int nrhs = b.size2();
    value_type *a_ptr = mtraits::storage( a ) ;
    int lda = n;
    value_type *b_ptr = mtraits::storage( b ) ; 
    int ldb = n;
    int& ipiv = *begin_ipiv ;
    int info = 0;
    assert( n == static_cast<int>(a.size2()) && n == static_cast<int>(b.size1()) );
    assert( std::distance( begin_ipiv, end_ipiv ) >= n );
    lapack< value_type >::getrs( &trans, &n, &nrhs, a_ptr, &lda, &ipiv, b_ptr, &ldb, &info );
    return info ;
  }

}}}

#endif // boost_numeric_lapackbindings_lapackbindings_hpp
