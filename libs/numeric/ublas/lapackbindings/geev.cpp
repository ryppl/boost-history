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

double norm(double arg)
{ return arg ; }

template < typename MatrixT1, typename MatrixT2 >
bool is_equal(const MatrixT1& m1, const MatrixT2& m2, double tolerance = 1e-8)
{
  size_t num_rows = m1.size1();
  size_t num_cols = m1.size2();
  double l_norm = 0 ;
  for(size_t i = 0; i < num_rows ; ++i )
    for(size_t j = 0; j < num_cols; ++j )
      l_norm += norm( m1(i,j) - m2(i,j) );
  return l_norm < tolerance ;
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

template < typename matrix_type >
void lower_tria_initialize(matrix_type& m)
{
  boost::minstd_rand generator(42u);
  boost::uniform_real< boost::minstd_rand > distribution(generator,0.0, 1.0);

  for(size_t i = 0 ; i < m.size1() ; ++i )
    for(size_t j = 0 ; j < i ; ++j ) 
      m(i,j) = distribution();
}

struct cmp_real_part
{
  bool operator()(std::complex<double> a, std::complex<double> b) const
  { return a.real() < b.real() ; }
};

template < typename T >
int geev_tst()
{
  for(size_t size_i = 10 ; size_i <= 10 ; size_i += 10 ) {
    // initialize T and D
    typedef triangular_matrix<T, numerics::lower> tria_matrix_type ;
    int info = 0;
    matrix< T, column_major > t(size_i, size_i);
    t.clear();
    lower_tria_initialize( t );
    matrix< double, column_major > eigen_values(size_i,size_i);
    eigen_values.clear();
    matrix_vector_range< matrix< T, column_major > > diagonal_t(t, numerics::range (0, size_i), numerics::range (0, size_i));
    for(size_t i = 0 ; i < size_i ; ++i ) {
      diagonal_t( i ) = 1.0;
      eigen_values(i,i) = i;
    }

    // calculate inverse of T
    matrix< T, column_major > lu_t = t;
    matrix< T, column_major > t_orig = t;

    std::vector< int > ipiv(size_i);
    info = getrf( lu_t, ipiv.begin(), ipiv.end() ); // inv_t == LU
    assert( ! info );
    matrix< T, column_major > inv_t = identity_matrix< T >( size_i );
    info = getrs('N', lu_t, ipiv.begin(), ipiv.end(), inv_t ); // inv_t == inverse
    assert( ! info );
    
    matrix< T, column_major > a = prod( inv_t, prod( eigen_values, t ) );
    matrix< T, column_major > a_orig = a;

    numerics::vector< std::complex< double > > w( size_i );
    matrix< T, column_major > vl(size_i,size_i);
    matrix< T, column_major > vr(size_i,size_i);
    geev('N', 'V', a, w, vl, vr );

    for(size_t i = 0 ; i < size_i ; ++i ) eigen_values( i, i ) = w[i].real();

    // check eigen-vectors : AX - Xdiag(D) ~= 0
    assert( is_equal( prod( a_orig, vr ) - prod( vr, eigen_values ), zero_matrix< T >( size_i ), 200.0 ) );

    // check eigen-values
    std::copy( w.begin(), w.end(), std::ostream_iterator< std::complex< double > >( std::cout, " " ) );
    std::cout << std::endl;
    std::sort( w.begin(), w.end(), cmp_real_part() );
    for(size_t i = 0 ; i < size_i ; ++i ) assert( is_equal( i * 1.0, w[i].real() ) );
  }
  return 0;
}

int main()
{

  geev_tst< double >();
  geev_tst< std::complex< double > >();

  std::cerr << "REGRESSION TEST SUCCEEDED !" << std::endl;
  return 0;
}
