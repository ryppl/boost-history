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
#include <boost/numeric/ublas/triangular.h>
#include <boost/numeric/ublas/blas.hpp>
#include <boost/numeric/ublas/lapack.hpp>
#include <boost/numeric/ublas/io.h>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>

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
  boost::minstd_rand generator(42u);
  boost::uniform_real< boost::minstd_rand > distribution(generator,0.0, 1000.0);

  typename matrix_type::iterator1 it1 = m.begin1();
  typename matrix_type::iterator1 end1 = m.end1();
  for( ; it1 != end1 ; ++it1 ) {
    typename matrix_type::iterator2 it2 = it1.begin();
    typename matrix_type::iterator2 end2 = it1.end();
    for( ; it2 != end2 ; ++it2 ) {
      *it2 = distribution();
    }
  }
}

int main()
{
  using namespace numerics ;

  for(size_t size_i = 10 ; size_i <= 10 ; size_i += 10 ) {
    typedef triangular_matrix<double, numerics::lower> matrix_type ;
    int info = 0;
    matrix_type t(size_i, size_i);
    initialize( t );
    matrix_type eigen_values(size_i,size_i);
    eigen_values.clear();
    matrix_vector_range< matrix_type > diagonal_t(t, numerics::range (0, size_i), numerics::range (0, size_i));
    for(size_t i = 0 ; i < size_i ; ++i ) {
      diagonal_t( i ) = 1.0;
      eigen_values(i,i) = i;
    }
    matrix< double, column_major > inv_t = identity_matrix< double >( size_i );

    matrix< double, column_major > lu_t = t;
    std::vector< int > ipiv(size_i);
    info = getrf( lu_t, ipiv.begin(), ipiv.end() ); // inv_t == LU
    assert( ! info );
    info = getrs('N', lu_t, ipiv.begin(), ipiv.end(), inv_t ); // inv_t == inverse
    assert( ! info );
    
    matrix< double, column_major > a = prod( inv_t, prod( eigen_values, t ) );

    std::vector< double > wr( size_i );
    std::vector< double > wi( size_i );
    matrix< double, column_major > vl(size_i,size_i);
    matrix< double, column_major > vr(size_i,size_i);
    geev('N', 'N', a, wr, wi, vl, vr );

    std::copy( wr.begin(), wr.end(), std::ostream_iterator< double >( std::cout, " " ) );
    std::cout << std::endl;
  }
  std::cerr << "REGRESSION TEST SUCCEEDED !" << std::endl;
  return 0;
}
