#ifndef boost_numeric_bindings_lapack_lapack_hpp
#define boost_numeric_bindings_lapack_lapack_hpp

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/traits/value_traits.hpp>
#include <boost/numeric/bindings/traits/transpose.hpp>
#include <boost/numeric/bindings/lapack/traits.hpp>

namespace boost { namespace numeric { namespace bindings { namespace lapack {

  template < typename matrix_type, typename IpivIterator >
  int getrf(matrix_type& a, IpivIterator begin_ipiv, IpivIterator end_ipiv)
  {
    typedef typename matrix_type::value_type                                                  value_type ;
    typedef boost::numeric::bindings::traits::matrix_traits< matrix_type >                    mtraits ; 
    typedef typename boost::numeric::bindings::traits::value_traits< value_type >::value_type bind_type ;

    int m = mtraits::size1( a ) ;
    int n = mtraits::size2( a ) ;
    const value_type *a_ptr = mtraits::storage( a ) ;
    int lda = m ;
    assert( std::distance( begin_ipiv, end_ipiv ) >= std::min( m,  n ) );
    int info = 0;
    int& ipiv_ptr = *begin_ipiv ;
    traits< value_type >::getrf( &m, &n, (bind_type*)a_ptr, &lda, &ipiv_ptr, &info );
    return info;
  }

  template <typename matrix_type, typename IntIterator >
  int getrs(char trans, matrix_type& a, IntIterator begin_ipiv, IntIterator end_ipiv, matrix_type& b)
  {
    typedef typename matrix_type::value_type                                                  value_type ;
    typedef boost::numeric::bindings::traits::matrix_traits< matrix_type >                    mtraits ; 
    typedef typename boost::numeric::bindings::traits::value_traits< value_type >::value_type bind_type ;

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
    traits< value_type >::getrs( &trans, &n, &nrhs, (bind_type*)a_ptr, &lda, &ipiv, (bind_type*)b_ptr, &ldb, &info );
    return info ;
  }

}}}}

#endif // boost_numeric_bindings_lapack_lapack_hpp
