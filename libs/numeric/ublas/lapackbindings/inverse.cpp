#ifdef BOOST_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>
#include <string>

#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/blas.hpp>
#include <boost/numeric/ublas/lapack.hpp>
#include <boost/numeric/ublas/io.h>

#include <boost/timer.hpp>

bool is_equal(const double& d1, const double& d2, double tolerance = 1e-8)
{ return fabs( d1 - d2 ) < tolerance ; }

template < typename matrix_type_1, typename matrix_type_2 >
bool is_equal(const matrix_type_1& m1, const matrix_type_2& m2, double tolerance = 1e-8)
{
  size_t num_rows = m1.size1();
  size_t num_cols = m1.size2();
  double norm = 0 ;
  for(size_t i = 0; i < num_rows ; ++i )
    for(size_t j = 0; j < num_cols; ++j )
      norm += fabs( m1(i,j) - m2(i,j) );
  return norm < tolerance ;
}

template < typename matrix_type >
void initialize(matrix_type& m)
{
  for(size_t i = 0 ; i < m.size1() ; ++i )
    for(size_t j = 0 ; j < m.size2() ; ++j )
      m(i,j) = ( ( i + 2 ) * ( j + 1 ) * (i*j + 22 ) ) ;
}

int main()
{
  int* ipiv = new int[3];
  numerics::matrix< double, numerics::column_major > a(3,3), lu( 3, 3 ), c(3,3);
  numerics::matrix< double, numerics::column_major > b( 3, 3 );
  b = numerics::identity_matrix< double >( 3 );
  lu.clear();
  lu(0,0) = 2;
  lu(0,1) = 3;
  lu(0,2) = 4.2;
  lu(1,0) = 2.2;
  lu(1,1) = 3.3;
  lu(1,2) = 1.1;
  lu(2,0) = 2.56;
  lu(2,1) = 10;
  lu(2,2) = 31;
  a = lu;

  int info;
  std::cout << "a : " << a << std::endl;
  std::cout << "b : " << b << std::endl;
  info = getrf( lu, ipiv, ipiv + 3);
  assert( ! info );
  std::cout << "ipiv : " << ipiv[0] << "," << ipiv[1] << "," << ipiv[2] << std::endl;

  info = getrs( 'N', lu, ipiv, ipiv + 3, b );
  assert( ! info );
  std::cout << "lu : " << lu << std::endl;
  std::cout << "b : " << b << std::endl;
  a = prod( a, b );
  std::cout << "should be identity : " << prod( a, b ) << std::endl;

  assert( is_equal( 1.0 , a(0,0) ) );
  assert( is_equal( 1.0 , a(1,1) ) );
  assert( is_equal( 1.0 , a(2,2) ) );
  assert( is_equal( 0.0 , a(0,1) ) );
  assert( is_equal( 0.0 , a(0,2) ) );
  assert( is_equal( 0.0 , a(1,0) ) );
  assert( is_equal( 0.0 , a(1,2) ) );
  assert( is_equal( 0.0 , a(2,0) ) );
  assert( is_equal( 0.0 , a(2,1) ) );

  std::cerr << "REGRESSION TEST SUCCEEDED !" << std::endl;
  return 0;
}
