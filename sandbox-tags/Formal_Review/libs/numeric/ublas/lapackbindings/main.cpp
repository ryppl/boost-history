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

int main (int argc, char *argv []) {
  using namespace numerics ;
  int info = 0;
  const int size = 6 ;

  matrix< double, column_major > m(size,size), lu(size,size), m_inv(size,size);
  initialize( m );
  lu = m ;
  m_inv = identity_matrix< double >(size);

  std::vector< int > ipiv(size);
  
  info = getrf( lu, ipiv.begin(), ipiv.end() );
  assert( ! info );

  info = getrs( TRANS_N, lu, ipiv.begin(), ipiv.end(), m_inv );
  assert( ! info );

  std::cout << prod( m, m_inv ) << std::endl;

  return 0;
}
