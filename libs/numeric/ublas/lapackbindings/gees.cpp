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

using namespace numerics ;

bool is_equal(const double& d1, const double& d2, double tolerance = 1e-8)
{ return fabs( d1 - d2 ) < tolerance ; }

template < typename T >
bool is_equal(const matrix< T >& m1, const matrix< T >& m2, double tolerance = 1e-8)
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
  boost::uniform_real< boost::minstd_rand > distribution(generator,0.0, 1.0);

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

struct cmp_real_part
{
  bool operator()(std::complex<double> a, std::complex<double> b) const
  { return a.real() < b.real() ; }
};

template < typename T >
int gees_tst()
{
  for(size_t size_i = 10 ; size_i <= 10 ; size_i += 10 ) {
    typedef triangular_matrix<T, numerics::lower> matrix_type ;
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
    matrix< T, column_major > inv_t = identity_matrix< T >( size_i );

    matrix< T, column_major > lu_t = t;
    std::vector< int > ipiv(size_i);
    info = getrf( lu_t, ipiv.begin(), ipiv.end() ); // inv_t == LU
    assert( ! info );
    info = getrs('N', lu_t, ipiv.begin(), ipiv.end(), inv_t ); // inv_t == inverse
    assert( ! info );
    
    matrix< T, column_major > a = prod( inv_t, prod( eigen_values, t ) );

    numerics::vector< std::complex< double > > w( size_i );
    matrix< T, column_major > vs(size_i,size_i);
    int sdim ;
    gees('V', 'N', a, sdim, w, vs );

    std::copy( w.begin(), w.end(), std::ostream_iterator< std::complex< double > >( std::cout, " " ) );
    std::cout << std::endl;
    std::sort( w.begin(), w.end(), cmp_real_part() );
    for(size_t i = 0 ; i < size_i ; ++i ) assert( is_equal( i * 1.0, w[i].real(), .5 ) );
  }
  return 0;
}

int main()
{

  gees_tst< double >();
  gees_tst< std::complex< double > >();

  std::cerr << "REGRESSION TEST SUCCEEDED !" << std::endl;
  return 0;
}
