#ifndef boost_numeric_bindings_lapack_lapack_hpp
#define boost_numeric_bindings_lapack_lapack_hpp

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/traits/value_traits.hpp>
#include <boost/numeric/bindings/traits/transpose.hpp>
#include <boost/numeric/bindings/lapack/traits.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>


namespace boost { namespace numeric { namespace bindings { namespace lapack {

  template < typename matrix_type, typename IpivIterator >
  int getrf(matrix_type& a, IpivIterator begin_ipiv, IpivIterator end_ipiv)
  {
    using namespace boost::numeric::bindings::traits ; 
    typedef typename matrix_type::value_type                                                  value_type ;
    typedef typename boost::numeric::bindings::traits::value_traits< value_type >::value_type bind_type ;

    int m = boost::numeric::bindings::traits::size1( a ) ;
    int n = boost::numeric::bindings::traits::size2( a ) ;
    value_type *a_ptr = boost::numeric::bindings::traits::matrix_storage( a ) ;
    const int lda = boost::numeric::bindings::traits::leading_dimension( a ) ;
    assert( std::distance( begin_ipiv, end_ipiv ) >= std::min( m,  n ) );
    int info = 0;
    int& ipiv_ptr = *begin_ipiv ;
    traits< value_type >::getrf( &m, &n, (bind_type*)a_ptr, &lda, &ipiv_ptr, &info );
    return info;
  }

  template <typename matrix_type, typename IntIterator, typename rhs_type >
  int getrs(char trans, const matrix_type& a, IntIterator begin_ipiv, IntIterator end_ipiv, rhs_type& b)
  {
    using namespace boost::numeric::bindings::traits ; 
    typedef typename matrix_type::value_type                                                  value_type ;
    typedef typename boost::numeric::bindings::traits::value_traits< value_type >::value_type bind_type ;

    BOOST_STATIC_ASSERT( (::boost::is_same< typename matrix_type::value_type, typename rhs_type::value_type >::value) );
    assert( trans == 'N' || trans == 'T' );
    int n = boost::numeric::bindings::traits::size1( a );
    int nrhs = boost::numeric::bindings::traits::size2( b );
    value_type *b_ptr = boost::numeric::bindings::traits::matrix_storage( b ) ; 
    const int ldb = boost::numeric::bindings::traits::leading_dimension( b ) ;
    const value_type *a_ptr = matrix_traits<const matrix_type>::storage( a ) ;
    const int lda = boost::numeric::bindings::traits::leading_dimension< const matrix_type > ( a ) ;
    const int& ipiv = const_cast< const int& >( *begin_ipiv ) ;
    int info = 0;
    assert( n == static_cast<int>(boost::numeric::bindings::traits::size2( a )) && n == static_cast<int>(boost::numeric::bindings::traits::size1 ( b ) ) );
    assert( std::distance( begin_ipiv, end_ipiv ) >= n );
    traits< value_type >::getrs( &trans, &n, &nrhs, (const bind_type*)a_ptr, &lda, &ipiv, (bind_type*)b_ptr, &ldb, &info );
    return info ;
  }


  template < typename matrix_type, typename IpivIterator, typename WorkIterator >
  int sytrf(char uplo, matrix_type& a, IpivIterator begin_ipiv, IpivIterator end_ipiv,
            WorkIterator begin_work, WorkIterator end_work)
  {
    typedef typename matrix_type::value_type                                                  value_type ;
    typedef typename boost::numeric::bindings::traits::value_traits< value_type >::value_type bind_type ;

    int n = bindings::traits::size1( a ) ;
    assert( boost::numeric::bindings::traits::size1( a ) == boost::numeric::bindings::traits::size2( a ) );
    value_type *a_ptr = boost::numeric::bindings::traits::matrix_storage( a ) ;
    const int lda = boost::numeric::bindings::traits::leading_dimension( a ) ;
    assert( std::distance( begin_ipiv, end_ipiv ) >= n );
    int info = 0;
    int& ipiv_ptr = *begin_ipiv ;
    value_type* work_ptr = begin_work;
    value_type* work_end_ptr = end_work;
    const int lwork = std::distance(work_ptr, work_end_ptr);
    traits< value_type >::sytrf( &uplo, &n, (bind_type*)a_ptr, &lda, &ipiv_ptr, (bind_type*)work_ptr, &lwork, &info );
    return info;
  }


  template <typename matrix_type, typename IntIterator, typename rhs_type >
  int sytrs(char uplo, const matrix_type& a, IntIterator begin_ipiv, IntIterator end_ipiv, rhs_type& b)
  {
    using namespace boost::numeric::bindings::traits ; 
    typedef typename matrix_type::value_type                                                  value_type ;
    typedef typename boost::numeric::bindings::traits::value_traits< value_type >::value_type bind_type ;

    BOOST_STATIC_ASSERT( (::boost::is_same< typename matrix_type::value_type, typename rhs_type::value_type >::value) );
    assert( uplo == 'U' || uplo == 'L' );
    int n = boost::numeric::bindings::traits::size1( a );
    int nrhs = boost::numeric::bindings::traits::size2( b );
    const value_type *a_ptr = matrix_traits<const matrix_type>::storage( a ) ;
    const int lda = boost::numeric::bindings::traits::leading_dimension< const matrix_type >( a ) ;
    value_type *b_ptr = boost::numeric::bindings::traits::matrix_storage( b ) ; 
    const int ldb = boost::numeric::bindings::traits::leading_dimension( b ) ;
    const int& ipiv = const_cast< const int& >( *begin_ipiv ) ;
    int info = 0;
    assert( n == static_cast<int>(boost::numeric::bindings::traits::size2( a )) && n == static_cast<int>(boost::numeric::bindings::traits::size1 ( b ) ) );
    assert( std::distance( begin_ipiv, end_ipiv ) >= n );
    traits< value_type >::sytrs( &uplo, &n, &nrhs, (const bind_type*)a_ptr, &lda, &ipiv, (bind_type*)b_ptr, &ldb, &info );
    return info ;
  }


  template < typename matrix_type >
  int potrf(char uplo, matrix_type& a)
  {
    typedef typename matrix_type::value_type                                                  value_type ;
    typedef typename boost::numeric::bindings::traits::value_traits< value_type >::value_type bind_type ;

    assert( uplo == 'U' || uplo == 'L' );

    int n = bindings::traits::size1( a ) ;
    assert( boost::numeric::bindings::traits::size1( a ) == boost::numeric::bindings::traits::size2( a ) );
    value_type *a_ptr = boost::numeric::bindings::traits::matrix_storage( a ) ;
    const int lda = boost::numeric::bindings::traits::leading_dimension( a ) ;
    int info = 0;
    traits< value_type >::potrf( &uplo, &n, (bind_type*)a_ptr, &lda, &info );
    return info;
  }


  template <typename matrix_type, typename rhs_type >
  int potrs(char uplo, const matrix_type& a, rhs_type& b)
  {
    using namespace boost::numeric::bindings::traits ; 
    typedef typename matrix_type::value_type                                                  value_type ;
    typedef typename boost::numeric::bindings::traits::value_traits< value_type >::value_type bind_type ;

    BOOST_STATIC_ASSERT( (::boost::is_same< typename matrix_type::value_type, typename rhs_type::value_type >::value) );
    assert( uplo == 'U' || uplo == 'L' );
    int n = boost::numeric::bindings::traits::size1( a );
    int nrhs = boost::numeric::bindings::traits::size2( b );
    const value_type *a_ptr = matrix_traits<const matrix_type>::storage( a ) ;
    const int lda = boost::numeric::bindings::traits::leading_dimension( a ) ;
    value_type *b_ptr = boost::numeric::bindings::traits::matrix_storage( b ) ; 
    const int ldb = boost::numeric::bindings::traits::leading_dimension( b ) ;
    int info = 0;
    assert( n == static_cast<int>(boost::numeric::bindings::traits::size2( a )) && n == static_cast<int>(boost::numeric::bindings::traits::size1 ( b ) ) );
    traits< value_type >::potrs( &uplo, &n, &nrhs, (const bind_type*)a_ptr, &lda, (bind_type*)b_ptr, &ldb, &info );
    return info ;
  }

}}}}

#endif // boost_numeric_bindings_lapack_lapack_hpp
