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
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>

#include <boost/timer.hpp>

bool is_equal(const double& d1, const double& d2, double tolerance = 1e-8)
{ return fabs( d1 - d2 ) < tolerance ; }

bool is_equal(const std::complex< double >& d1, const std::complex< double >& d2, double tolerance = 1e-8)
{ return std::norm( d1 - d2 ) < tolerance ; }

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

template < typename T >
void tst_inverse()
{
  for(size_t size_i = 10; size_i < 100 ; size_i += 10 ) {
    int* ipiv = new int[size_i];
    numerics::matrix< double, numerics::column_major > a(size_i,size_i), lu( size_i, size_i ), c(size_i,size_i);
    numerics::matrix< double, numerics::column_major > b( size_i, size_i );
    b = numerics::identity_matrix< double >( size_i );
    initialize( a );
    lu = a;

    int info;
    info = getrf( lu, ipiv, ipiv + size_i);
    assert( ! info );
    
    info = getrs( 'N', lu, ipiv, ipiv + size_i, b );
    assert( ! info );

    a = prod( a, b );
    for(size_t i = 0; i < size_i ; ++i ) 
      for(size_t j = 0; j < size_i ; ++j ) {
        T ref = 0.0 ;
        if ( i == j ) ref = 1.0;
        assert( is_equal( ref, a(i,j) ) );
      }
  }
}

int main()
{
  tst_inverse< double >();
  tst_inverse< std::complex< double > >();
  std::cerr << "REGRESSION TEST SUCCEEDED !" << std::endl;
  return 0;
}
