#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>

#include <math/test/blas.hpp>
#include <math/blas3.hpp>

template < typename ValueType, typename MatrixTypeA, typename MatrixTypeB, typename MatrixTypeC >
void test_gemm(std::ostream& os, int runs, int runs_i, int size, int size_i, ValueType alpha, ValueType beta, MatrixTypeA &a, MatrixTypeB& b, MatrixTypeC &c_native, MatrixTypeC &c_toblas)
{
  typedef ValueType value_type ;

  boost::timer t ;
  for(int i = 0 ; i < runs_i ; ++i ) c_native = ( alpha * prod(a,b) ) + ( beta * c_native );
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  t.restart() ;
  for(int i = 0 ; i < runs_i ; ++i ) numerics::gemm( alpha, a, b, beta, c_toblas );
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  check( c_native, c_toblas );
}

struct gemm_matrix_matrix_matrix
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    T alpha = 1 / ( size_i * 10 ), beta = 1.0 ;
    numerics::matrix< T > a( size_i, size_i );
    random_initialise_matrix( a );
    numerics::matrix< T > b( a );
    numerics::matrix< T > c_native( a );
    numerics::matrix< T > c_toblas( a );
      
    test_gemm( os, runs, runs_i, size, size_i, alpha, beta, a, b, c_native, c_toblas );
  }
};

int main (int argc, char *argv []) 
{
  int runs = 10 ; // 10000000 ;
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
      std::cerr <<         "gemm_matrix_matrix_matrix_double" << std::endl ;
      std::ofstream stream("gemm_matrix_matrix_matrix_double");
      loop< double, gemm_matrix_matrix_matrix >( stream, start, step, stop, runs, gemm_matrix_matrix_matrix() ) ;
    } 
    
    {
      std::cerr <<         "gemm_matrix_matrix_matrix_double_complex" << std::endl ;
      std::ofstream stream("gemm_matrix_matrix_matrix_double_complex");
      loop< std::complex<double>, gemm_matrix_matrix_matrix >( stream, start, step, stop, runs, gemm_matrix_matrix_matrix() ) ;
    }
  }
}
