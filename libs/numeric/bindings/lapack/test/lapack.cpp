#include "../../blas/test/blas.hpp"
#include <boost/numeric/bindings/lapack/lapack.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>

template < typename value_type >
void test_getrf_getrs()
{
  size_t size = 5 ;
  numerics::matrix< value_type, numerics::column_major > a(size,size) ;
  random_initialise_matrix( a ) ;
  numerics::matrix< value_type, numerics::column_major > lu( a ) ;

  numerics::matrix< value_type, numerics::column_major > b(size,1) ;
  random_initialise_matrix( b ) ;

  numerics::matrix< value_type, numerics::column_major > x( b ) ;

  std::vector< int > ipiv( size ) ;

  boost::numeric::bindings::lapack::getrf( lu, ipiv.begin(), ipiv.end() ) ;

  boost::numeric::bindings::lapack::getrs( 'N', lu, ipiv.begin(), ipiv.end(), x ) ;

  std::cout << prod(a,x) - b << std::endl ; 
}

int main()
{
  test_getrf_getrs< double >() ;
  test_getrf_getrs< std::complex< double > >() ;
  return 0 ;
}
