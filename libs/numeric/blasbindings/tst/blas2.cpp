#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>

#include "blas.hpp"
#include <boost/numeric/blasbindings/blas_traits/ublas_vector.hpp>
#include <boost/numeric/blasbindings/blas_traits/ublas_matrix.hpp>
#include <boost/numeric/blasbindings/blas2.hpp>
#include <boost/numeric/blasbindings/transpose.hpp>
#include <boost/numeric/ublas/io.hpp>

template < typename ValueType, typename MatrixType, typename VectorType >
void test_gemv(std::ostream& os, int runs, int runs_i, int size, int size_i, char c, ValueType alpha, ValueType beta, MatrixType &a, VectorType& x, VectorType &y_native, VectorType &y_toblas)
{
  typedef typename VectorType::value_type value_type ;

  std::cout << a << std::endl;
  std::cout << x << std::endl;
  std::cout << y_native << std::endl;
  std::cout << y_toblas << std::endl;

  boost::timer t ;
  if ( c == boost::numeric::blasbindings::NO_TRANSPOSE )    for(int i = 0 ; i < runs_i ; ++i ) y_native += alpha * numerics::prod( a, x) ;
  else if ( c == boost::numeric::blasbindings::TRANSPOSE )  for(int i = 0 ; i < runs_i ; ++i ) y_native += alpha * numerics::prod( trans(a), x) ;
  else if ( c == boost::numeric::blasbindings::CONJUGATE )  for(int i = 0 ; i < runs_i ; ++i ) y_native += alpha * numerics::prod( herm(a), x) ;
  else assert( 0 ) ;
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  t.restart() ;
  for(int i = 0 ; i < runs_i ; ++i ) boost::numeric::blasbindings::gemv( c, alpha, a, x, beta, y_toblas ) ;
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  check( y_native.begin(), y_native.end(), y_toblas.begin() );
}

struct gemv_matrix_vector_vector
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    runs_i = std::max( 1, runs_i / size_i ) ;

    T alpha = 1 / ( size_i * 10 ), beta = 1.0 ;
    random_initialise( alpha );
    numerics::matrix< T, numerics::column_major > a(size_i,size_i) ;
    random_initialise_matrix( a ) ;
    numerics::vector< T > x( size_i ); 
    random_initialise_vector( x) ;
    numerics::vector< T > y_native( x ) ;
    numerics::vector< T > y_toblas( x ) ;
      
    test_gemv( os, runs, runs_i, size, size_i, boost::numeric::blasbindings::NO_TRANSPOSE, alpha, beta, a, x, y_native, y_toblas );
  }
};

struct gemv_trans_matrix_vector_vector
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    runs_i = std::max( 1, runs_i / size_i ) ;

    T alpha = 1 / ( size_i * 10 ), beta = 1.0 ;
    random_initialise( alpha );
    numerics::matrix< T, numerics::column_major > a(size_i,size_i) ;
    random_initialise_matrix( a ) ;
    numerics::vector< T > x( size_i ); 
    random_initialise_vector( x) ;
    numerics::vector< T > y_native( x ) ;
    numerics::vector< T > y_toblas( x ) ;
      
    test_gemv( os, runs, runs_i, size, size_i, boost::numeric::blasbindings::TRANSPOSE, alpha, beta, a, x, y_native, y_toblas );
  }
};

struct gemv_conj_matrix_vector_vector
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    runs_i = std::max( 1, runs_i / size_i ) ;

    T alpha = 1 / ( size_i * 10 ), beta = 1.0 ;
    random_initialise( alpha );
    numerics::matrix< T, numerics::column_major > a(size_i,size_i) ;
    random_initialise_matrix( a ) ;
    numerics::vector< T > x( size_i ); 
    random_initialise_vector( x) ;
    numerics::vector< T > y_native( x ) ;
    numerics::vector< T > y_toblas( x ) ;
      
    test_gemv( os, runs, runs_i, size, size_i, boost::numeric::blasbindings::CONJUGATE, alpha, beta, a, x, y_native, y_toblas );
  }
};

struct gemv_matrix_range_vector_vector
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    runs_i = std::max( 1, runs_i / size_i ) ;

    T alpha = 1 / ( size_i * 10 ), beta = 1.0 ;
    random_initialise( alpha );
    numerics::matrix< T, numerics::column_major > a(size_i * 2,size_i * 2) ;
    random_initialise_matrix( a ) ;
    int start = size_i / 2 ;
    int stop = start + size_i ;
    numerics::matrix_range< numerics::matrix< T, numerics::column_major > > mr( a, numerics::range( start, stop ), numerics::range( start, stop ) ) ;
    numerics::vector< T > x( size_i ); 
    random_initialise_vector( x) ;
    numerics::vector< T > y_native( x ) ;
    numerics::vector< T > y_toblas( x ) ;
      
    test_gemv( os, runs, runs_i, size, size_i, boost::numeric::blasbindings::NO_TRANSPOSE, alpha, beta, mr, x, y_native, y_toblas );
  }
};

int main (int argc, char *argv []) 
{
  int runs = 1 ; // 10000000 ;
  int start = 1 ;
  int step  = 50 ;
  int stop  = 10 ; // 10000 ;

  std::cerr << "\npeak float\n";
  peak<float> () ( runs );

  std::cerr << "\npeak double\n";
  peak<double> () ( runs );

  std::cerr << "\nstd:complex<float>\n";
  peak<std::complex<float> > () ( runs );

  std::cerr << "\nstd:complex<double>\n";
  peak<std::complex<double> > () ( runs );

  if (argc > 1) {
    int scale = atoi(argv [1]);
    runs *= scale ;
  }

  {
    {
      std::cerr <<         "gemv_matrix_vector_vector_double" << std::endl ;
      std::ofstream stream("gemv_matrix_vector_vector_double");
      loop< double, gemv_matrix_vector_vector >( stream, start, step, stop, runs, gemv_matrix_vector_vector() ) ;
    } 
    
    {
      std::cerr <<         "gemv_matrix_vector_vector_double_complex" << std::endl ;
      std::ofstream stream("gemv_matrix_vector_vector_double_complex");
      loop< std::complex<double>, gemv_matrix_vector_vector >( stream, start, step, stop, runs, gemv_matrix_vector_vector() ) ;
    }

    {
      std::cerr <<         "gemv_trans_matrix_vector_vector_double" << std::endl ;
      std::ofstream stream("gemv_trans_matrix_vector_vector_double");
      loop< double, gemv_trans_matrix_vector_vector >( stream, start, step, stop, runs, gemv_trans_matrix_vector_vector() ) ;
    } 
    
    {
      std::cerr <<         "gemv_trans_matrix_vector_vector_double_complex" << std::endl ;
      std::ofstream stream("gemv_trans_matrix_vector_vector_double_complex");
      loop< std::complex<double>, gemv_trans_matrix_vector_vector >( stream, start, step, stop, runs, gemv_trans_matrix_vector_vector() ) ;
    }

    {
      std::cerr <<         "gemv_conj_matrix_vector_vector_double_complex" << std::endl ;
      std::ofstream stream("gemv_conj_matrix_vector_vector_double_complex");
      loop< std::complex<double>, gemv_conj_matrix_vector_vector >( stream, start, step, stop, runs, gemv_conj_matrix_vector_vector() ) ;
    }

    {
      std::cerr <<         "gemv_matrix_range_vector_vector_double" << std::endl ;
      std::ofstream stream("gemv_matrix_range_vector_vector_double");
      loop< double, gemv_matrix_range_vector_vector >( stream, start, step, stop, runs, gemv_matrix_range_vector_vector() ) ;
    } 
    
    {
      std::cerr <<         "gemv_matrix_range_vector_vector_double_complex" << std::endl ;
      std::ofstream stream("gemv_matrix_range_vector_vector_double_complex");
      loop< std::complex<double>, gemv_matrix_range_vector_vector >( stream, start, step, stop, runs, gemv_matrix_range_vector_vector() ) ;
    }
  }

  return 0 ;
}


