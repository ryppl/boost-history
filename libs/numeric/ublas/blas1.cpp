#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>

#include "blas.hpp"
#include <boost/numeric/ublas/blas1.hpp>

template < typename VectorType >
void test_scal(std::ostream& os, int runs, int runs_i, int size, int size_i, typename VectorType::value_type alpha, VectorType& vector_native, VectorType& vector_toblas)
{
  typedef typename VectorType::value_type value_type ;

  std::cout << "\ntest_scal : \n";
  std::cout << alpha << "\n";
  std::copy( vector_native.begin(), vector_native.end(), std::ostream_iterator< value_type >( std::cout, " " ) ); std::cout << "\n";
  std::copy( vector_toblas.begin(), vector_toblas.end(), std::ostream_iterator< value_type >( std::cout, " " ) ); std::cout << "\n";
  std::cout << std::endl;

  boost::timer t ;
  for(int i = 0 ; i < runs_i ; ++i ) vector_native *= alpha ;
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  t.restart() ;
  for(int i = 0 ; i < runs_i ; ++i ) numerics::scal( alpha, vector_toblas );
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  std::cout << "\n";
  std::copy( vector_native.begin(), vector_native.end(), std::ostream_iterator< value_type >( std::cout, " " ) ); std::cout << "\n";
  std::copy( vector_toblas.begin(), vector_toblas.end(), std::ostream_iterator< value_type >( std::cout, " " ) ); std::cout << "\n";
  check( vector_native.begin(), vector_native.end(), vector_toblas.begin() );
}

struct scal_vector
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > vector_native( size_i ); 
    random_initialise_vector( vector_native );
    numerics::vector< T > vector_toblas( vector_native ) ;
    T alpha = assign_multiplier< T >()() ; // 1.0000002 ; // otherwise the accumulated result of the range will result in 0.0 or over/under-flow
      
    test_scal( os, runs, runs_i, size, size_i, alpha, vector_native, vector_toblas );
  }
};

struct scal_vector_range
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > vector_native( size ); 
    random_initialise_vector( vector_native );
    numerics::vector< T > vector_toblas( vector_native ) ;
    T alpha = 1.0000002 ;

    std::cout << std::endl;
    std::cout << static_cast< T >( std::numeric_limits< float >::max() ) << std::endl;
    std::cout << 100.0 * 10 * runs_i << std::endl;
    std::cout << runs_i << std::endl;
    std::cout << alpha << std::endl;
    int start = std::max( 0, ( size / 2 ) - size_i );
    int stop  = start + size_i ;
    numerics::vector_range< numerics::vector< T > > native_range( vector_native, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > toblas_range( vector_toblas, numerics::range(start,stop) );
    std::copy( native_range.begin(), native_range.end(), std::ostream_iterator< T >( std::cout, " " ) ) ; std::cout << std::endl;
    std::copy( toblas_range.begin(), toblas_range.end(), std::ostream_iterator< T >( std::cout, " " ) ) ; std::cout << std::endl;
      
    test_scal( os, runs, runs_i, size, size_i, alpha, native_range, toblas_range );
  }
};

struct scal_vector_slice
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > vector_native( size ); 
    random_initialise_vector( vector_native );
    numerics::vector< T > vector_toblas( vector_native ) ;
    T alpha = 1.0000002 ;

    int stride = size / size_i ;
    numerics::vector_slice< numerics::vector< T > > native_slice( vector_native, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > toblas_slice( vector_toblas, numerics::slice(0,stride,size_i) );
      
    test_scal( os, runs, runs_i, size, size_i, alpha, native_slice, toblas_slice );
  }
};

struct scal_matrix_row_row_major
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    typedef numerics::matrix< T, numerics::row_major > matrix_type ;

    matrix_type matrix_native( 3, size_i);
    random_initialise_matrix( matrix_native );
    matrix_type matrix_toblas( matrix_native ) ;

    numerics::matrix_row< matrix_type > native_row( matrix_native, 1 );
    numerics::matrix_row< matrix_type > toblas_row( matrix_toblas, 1 );
    T alpha = assign_multiplier< T >()() ; // 1.0000002 ; // otherwise the accumulated result of the range will result in 0.0 or over/under-flow
      
    test_scal( os, runs, runs_i, size, size_i, alpha, native_row, toblas_row );
  }
};

struct scal_matrix_row_column_major
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    typedef numerics::matrix< T, numerics::column_major > matrix_type ;

    matrix_type matrix_native( 3, size_i);
    random_initialise_matrix( matrix_native );
    matrix_type matrix_toblas( matrix_native ) ;

    numerics::matrix_row< matrix_type > native_row( matrix_native, 1 );
    numerics::matrix_row< matrix_type > toblas_row( matrix_toblas, 1 );
    T alpha = assign_multiplier< T >()() ; // 1.0000002 ; // otherwise the accumulated result of the range will result in 0.0 or over/under-flow
      
    test_scal( os, runs, runs_i, size, size_i, alpha, native_row, toblas_row );
  }
};

struct scal_matrix_column_row_major
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    typedef numerics::matrix< T, numerics::row_major > matrix_type ;

    matrix_type matrix_native( size_i, 3 );
    random_initialise_matrix( matrix_native );
    matrix_type matrix_toblas( matrix_native ) ;

    numerics::matrix_column< matrix_type > native_column( matrix_native, 1 );
    numerics::matrix_column< matrix_type > toblas_column( matrix_toblas, 1 );
    T alpha = assign_multiplier< T >()() ; // 1.0000002 ; // otherwise the accumulated result of the range will result in 0.0 or over/under-flow
      
    test_scal( os, runs, runs_i, size, size_i, alpha, native_column, toblas_column );
  }
};

struct scal_matrix_column_column_major
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    typedef numerics::matrix< T, numerics::column_major > matrix_type ;

    matrix_type matrix_native( size_i, 3);
    random_initialise_matrix( matrix_native );
    matrix_type matrix_toblas( matrix_native ) ;

    numerics::matrix_column< matrix_type > native_column( matrix_native, 1 );
    numerics::matrix_column< matrix_type > toblas_column( matrix_toblas, 1 );
    T alpha = assign_multiplier< T >()() ; // 1.0000002 ; // otherwise the accumulated result of the range will result in 0.0 or over/under-flow
      
    test_scal( os, runs, runs_i, size, size_i, alpha, native_column, toblas_column );
  }
};

template < typename VectorTypeX, typename VectorTypeY >
void test_axpy(std::ostream& os, int runs, int runs_i, int size, int size_i, typename VectorTypeX::value_type alpha, const VectorTypeX& x, VectorTypeY& y_native, VectorTypeY& y_toblas)
{
  typedef typename VectorTypeX::value_type value_type ;

  boost::timer t ;
  for(int i = 0 ; i < runs_i ; ++i ) y_native += alpha * x;
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  t.restart() ;
  for(int i = 0 ; i < runs_i ; ++i ) numerics::axpy( alpha, x, y_toblas );
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  check( y_native.begin(), y_native.end(), y_toblas.begin() ) ;
}

struct axpy_vector
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > x( size_i ); 
    random_initialise_vector( x );
    numerics::vector< T > y_native( x ) ;
    numerics::vector< T > y_toblas( x ) ;
    T alpha = std::numeric_limits< T >::epsilon() ;
      
    test_axpy( os, runs, runs_i, size, size_i, alpha, x, y_native, y_toblas );
  }
};

struct axpy_vector_range
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > x( size ); 
    random_initialise_vector( x );
    numerics::vector< T > y_native( x ) ;
    numerics::vector< T > y_toblas( x ) ;
    T alpha = std::numeric_limits< T >::epsilon() ;

    int start = std::max( 0, ( size / 2 ) - size_i );
    int stop  = start + size_i ;
    numerics::vector_range< numerics::vector< T > > x_range( x, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > y_native_range( y_native, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > y_toblas_range( y_toblas, numerics::range(start,stop) );
      
    test_axpy( os, runs, runs_i, size, size_i, alpha, x_range, y_native_range, y_toblas_range);
  }
};

struct axpy_vector_slice
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > x( size ); 
    random_initialise_vector( x );
    numerics::vector< T > y_native( x ) ;
    numerics::vector< T > y_toblas( x ) ;
    T alpha = std::numeric_limits< T >::epsilon() ;

    int stride = size / size_i ;
    numerics::vector_slice< numerics::vector< T > > x_native_slice( x, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > y_native_slice( y_native, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > y_toblas_slice( y_toblas, numerics::slice(0,stride,size_i) );
      
    test_axpy( os, runs, runs_i, size, size_i, alpha, x_native_slice, y_native_slice, y_toblas_slice ) ;
  }
};

struct axpy_vector_range_range_vector_slice_range
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > x( size ); 
    random_initialise_vector( x );
    numerics::vector< T > y_native( x ) ;
    numerics::vector< T > y_toblas( x ) ;
    T alpha = std::numeric_limits< T >::epsilon() ;

    int start = std::max( 0, ( size / 2 ) - size_i );
    int stop  = start + size_i ;
    numerics::vector_range< numerics::vector< T > > x_range( x, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector_range< numerics::vector< T > > > x_range_range( x_range, numerics::range(0,size_i) );
    numerics::vector_range< numerics::vector< T > > y_native_range( y_native, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > y_toblas_range( y_toblas, numerics::range(start,stop) );
    numerics::vector_slice< numerics::vector_range< numerics::vector< T > > > y_native_slice_range( y_native_range, numerics::slice(0,1,size_i) );
    numerics::vector_slice< numerics::vector_range< numerics::vector< T > > > y_toblas_slice_range( y_toblas_range, numerics::slice(0,1,size_i) );
      
    test_axpy( os, runs, runs_i, size, size_i, alpha, x_range_range, y_native_slice_range, y_toblas_slice_range ) ;
  }
};

struct dot_functor
{
  template < typename vector_type >
  typename vector_type::value_type operator()(const vector_type &x, const vector_type &y) const
  { return numerics::dot( x, y ) ; }
};

struct dotu_functor
{
  template < typename vector_type >
  typename vector_type::value_type operator()(const vector_type &x, const vector_type &y) const
  { return numerics::dotu( x, y ) ; }
};

template < typename VectorType, typename Functor >
void test_dot(std::ostream& os, int runs, int runs_i, int size, int size_i, VectorType& x_native, VectorType& y_native, VectorType& x_toblas, VectorType& y_toblas, Functor functor)
{
  typedef typename VectorType::value_type value_type ;

  value_type result_native = value_type(), result_toblas = value_type();

  std::copy( x_native.begin(), x_native.end(), std::ostream_iterator< value_type >( std::cout, " " ) ); std::cout << std::endl;
  std::copy( y_native.begin(), y_native.end(), std::ostream_iterator< value_type >( std::cout, " " ) ); std::cout << std::endl;
  std::copy( x_toblas.begin(), x_toblas.end(), std::ostream_iterator< value_type >( std::cout, " " ) ); std::cout << std::endl;
  std::copy( y_toblas.begin(), y_toblas.end(), std::ostream_iterator< value_type >( std::cout, " " ) ); std::cout << std::endl;

  boost::timer t ;
  for(int i = 0 ; i < runs_i ; ++i ) result_native = numerics::inner_prod( x_native, y_native );
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  t.restart() ;
  for(int i = 0 ; i < runs_i ; ++i ) result_toblas = functor( x_toblas, y_toblas );
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  check( result_native, result_toblas );
}

struct dot_vector
{
  template < typename T, typename Functor >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i, Functor functor)
  {
    numerics::vector< T > x_native( size_i ); 
    random_initialise_vector( x_native );
    numerics::vector< T > y_native( x_native ) ;
    numerics::vector< T > x_toblas( x_native ) ;
    numerics::vector< T > y_toblas( x_native ) ;
      
    test_dot( os, runs, runs_i, size, size_i, x_native, y_native, x_toblas, y_toblas, functor );
  }
};

struct dot_vector_range
{
  template < typename T, typename Functor >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i, Functor functor)
  {
    numerics::vector< T > x_native( size ); 
    random_initialise_vector( x_native );
    numerics::vector< T > y_native( x_native ) ;
    numerics::vector< T > x_toblas( x_native ) ;
    numerics::vector< T > y_toblas( x_native ) ;

    int start = std::max( 0, ( size / 2 ) - size_i );
    int stop  = start + size_i ;
    numerics::vector_range< numerics::vector< T > > x_native_range( x_native, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > y_native_range( y_native, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > x_toblas_range( x_toblas, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > y_toblas_range( y_toblas, numerics::range(start,stop) );
      
    test_dot( os, runs, runs_i, size, size_i, x_native_range, y_native_range, x_toblas_range, y_toblas_range, functor);
  }
};

struct dot_vector_slice
{
  template < typename T, typename Functor >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i, Functor functor)
  {
    numerics::vector< T > x_native( size ); 
    random_initialise_vector( x_native );
    numerics::vector< T > y_native( x_native ) ;
    numerics::vector< T > x_toblas( x_native ) ;
    numerics::vector< T > y_toblas( x_native ) ;

    int stride = size / size_i ;
    numerics::vector_slice< numerics::vector< T > > x_native_slice( x_native, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > y_native_slice( y_native, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > x_toblas_slice( x_toblas, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > y_toblas_slice( y_toblas, numerics::slice(0,stride,size_i) );
      
    test_dot( os, runs, runs_i, size, size_i, x_native_slice, y_native_slice, x_toblas_slice, y_toblas_slice, functor ) ;
  }
};

template < typename VectorType >
void test_dotc(std::ostream& os, int runs, int runs_i, int size, int size_i, VectorType& x_native, VectorType& y_native, VectorType& x_toblas, VectorType& y_toblas)
{
  typedef typename VectorType::value_type value_type ;

  value_type result_native, result_toblas ;

  boost::timer t ;
  for(int i = 0 ; i < runs_i ; ++i ) result_native = numerics::inner_prod( numerics::conj(x_native), y_native );
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  t.restart() ;
  for(int i = 0 ; i < runs_i ; ++i ) result_toblas = numerics::dot( numerics::conj(x_toblas), y_toblas );
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  check( result_native, result_toblas );
}

struct dotc_vector
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > x_native( size_i ); 
    random_initialise_vector( x_native );
    numerics::vector< T > y_native( x_native ) ;
    numerics::vector< T > x_toblas( x_native ) ;
    numerics::vector< T > y_toblas( x_native ) ;
      
    test_dotc( os, runs, runs_i, size, size_i, x_native, y_native, x_toblas, y_toblas );
  }
};

struct dotc_vector_range
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > x_native( size ); 
    random_initialise_vector( x_native );
    numerics::vector< T > y_native( x_native ) ;
    numerics::vector< T > x_toblas( x_native ) ;
    numerics::vector< T > y_toblas( x_native ) ;

    int start = std::max( 0, ( size / 2 ) - size_i );
    int stop  = start + size_i ;
    numerics::vector_range< numerics::vector< T > > x_native_range( x_native, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > y_native_range( y_native, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > x_toblas_range( x_toblas, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > y_toblas_range( y_toblas, numerics::range(start,stop) );
      
    test_dotc( os, runs, runs_i, size, size_i, x_native_range, y_native_range, x_toblas_range, y_toblas_range);
  }
};

struct dotc_vector_slice
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > x_native( size ); 
    random_initialise_vector( x_native );
    numerics::vector< T > y_native( x_native ) ;
    numerics::vector< T > x_toblas( x_native ) ;
    numerics::vector< T > y_toblas( x_native ) ;

    int stride = size / size_i ;
    numerics::vector_slice< numerics::vector< T > > x_native_slice( x_native, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > y_native_slice( y_native, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > x_toblas_slice( x_toblas, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > y_toblas_slice( y_toblas, numerics::slice(0,stride,size_i) );
      
    test_dotc( os, runs, runs_i, size, size_i, x_native_slice, y_native_slice, x_toblas_slice, y_toblas_slice ) ;
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
      std::cerr <<         "scal_vector_double" << std::endl ;
      std::ofstream stream("scal_vector_double");
      loop< double, scal_vector >( stream, start, step, stop, runs, scal_vector() ) ;
    }
    
    {
      std::cerr <<         "scal_vector_double_complex" << std::endl ;
      std::ofstream stream("scal_vector_double_complex");
      loop< std::complex< double >, scal_vector >( stream, start, step, stop, runs, scal_vector() ) ;
    }
    
    {
      std::cerr <<         "scal_vector_range_double" << std::endl ;
      std::ofstream stream("scal_vector_range_double");
      loop< double, scal_vector_range >( stream, start, step, stop, runs, scal_vector_range() ) ;
    }
    
    {
      std::cerr <<         "scal_vector_range_double_complex" << std::endl ;
      std::ofstream stream("scal_vector_range_double_complex");
      loop< std::complex< double >, scal_vector_range >( stream, start, step, stop, runs, scal_vector_range() ) ;
    }
    
    {
      std::cerr <<         "scal_vector_slice_double" << std::endl ;
      std::ofstream stream("scal_vector_slice_double");
      loop< double, scal_vector_slice >( stream, start, step, stop, runs, scal_vector_slice() ) ;
    }
    
    {
      std::cerr <<         "scal_vector_slice_double_complex" << std::endl ;
      std::ofstream stream("scal_vector_slice_double_complex");
      loop< std::complex< double >, scal_vector_slice >( stream, start, step, stop, runs, scal_vector_slice() ) ;
    }

    {
      std::cerr <<         "scal_matrix_row_row_major_double" << std::endl ;
      std::ofstream stream("scal_matrix_row_row_major_double");
      loop< double, scal_matrix_row_row_major >( stream, start, step, stop, runs, scal_matrix_row_row_major() ) ;
    }
    
    {
      std::cerr <<         "scal_matrix_row_row_major_double_complex" << std::endl ;
      std::ofstream stream("scal_matrix_row_row_major_double_complex");
      loop< std::complex< double >, scal_matrix_row_row_major >( stream, start, step, stop, runs, scal_matrix_row_row_major() ) ;
    }

    {
      std::cerr <<         "scal_matrix_row_column_major_double" << std::endl ;
      std::ofstream stream("scal_matrix_row_column_major_double");
      loop< double, scal_matrix_row_column_major >( stream, start, step, stop, runs, scal_matrix_row_column_major() ) ;
    }
    
    {
      std::cerr <<         "scal_matrix_row_column_major_double_complex" << std::endl ;
      std::ofstream stream("scal_matrix_row_column_major_double_complex");
      loop< std::complex< double >, scal_matrix_row_column_major >( stream, start, step, stop, runs, scal_matrix_row_column_major() ) ;
    }

    {
      std::cerr <<         "scal_matrix_column_row_major_double" << std::endl ;
      std::ofstream stream("scal_matrix_column_row_major_double");
      loop< double, scal_matrix_column_row_major >( stream, start, step, stop, runs, scal_matrix_column_row_major() ) ;
    }
    
    {
      std::cerr <<         "scal_matrix_column_row_major_double_complex" << std::endl ;
      std::ofstream stream("scal_matrix_column_row_major_double_complex");
      loop< std::complex< double >, scal_matrix_column_row_major >( stream, start, step, stop, runs, scal_matrix_column_row_major() ) ;
    }

    {
      std::cerr <<         "scal_matrix_column_column_major_double" << std::endl ;
      std::ofstream stream("scal_matrix_column_column_major_double");
      loop< double, scal_matrix_column_column_major >( stream, start, step, stop, runs, scal_matrix_column_column_major() ) ;
    }
    
    {
      std::cerr <<         "scal_matrix_column_column_major_double_complex" << std::endl ;
      std::ofstream stream("scal_matrix_column_column_major_double_complex");
      loop< std::complex< double >, scal_matrix_column_column_major >( stream, start, step, stop, runs, scal_matrix_column_column_major() ) ;
    }
  }

  {
    {
      std::cerr <<         "axpy_vector_double" << std::endl ;
      std::ofstream stream("axpy_vector_double");
      loop< double, axpy_vector >( stream, start, step, stop, runs, axpy_vector() );
    }

    {
      std::cerr <<         "axpy_vector_double_complex" << std::endl ;
      std::ofstream stream("axpy_vector_double_complex");
      loop< std::complex< double >, axpy_vector >( stream, start, step, stop, runs, axpy_vector() );
    }

    {
      std::cerr <<         "axpy_vector_range_double" << std::endl ;
      std::ofstream stream("axpy_vector_range_double");
      loop< double, axpy_vector_range >( stream, start, step, stop, runs, axpy_vector_range() ) ;
    }
    
    {
      std::cerr <<         "axpy_vector_range_double_complex" << std::endl ;
      std::ofstream stream("axpy_vector_range_double_complex");
      loop< std::complex< double >, axpy_vector_range >( stream, start, step, stop, runs, axpy_vector_range() ) ;
    }

    {
      std::cerr <<         "axpy_vector_slice_double" << std::endl ;
      std::ofstream stream("axpy_vector_slice_double");
      loop< double, axpy_vector_slice >( stream, start, step, stop, runs, axpy_vector_slice() ) ;
    }
    
    {
      std::cerr <<         "axpy_vector_slice_double_complex" << std::endl ;
      std::ofstream stream("axpy_vector_slice_double_complex");
      loop< std::complex< double >, axpy_vector_slice >( stream, start, step, stop, runs, axpy_vector_slice() ) ;
    }

    {
      std::cerr <<         "axpy_vector_range_range_vector_slice_range_double" << std::endl ;
      std::ofstream stream("axpy_vector_range_range_vector_slice_range_double");
      loop< double, axpy_vector_range_range_vector_slice_range >( stream, start, step, stop, runs, axpy_vector_range_range_vector_slice_range() ) ;
    }

    {
      std::cerr <<         "axpy_vector_range_range_vector_slice_range_double_complex" << std::endl ;
      std::ofstream stream("axpy_vector_range_range_vector_slice_range_double_complex");
      loop< std::complex< double >, axpy_vector_range_range_vector_slice_range >( stream, start, step, stop, runs, axpy_vector_range_range_vector_slice_range() ) ;
    }
  }

  {
    {
      std::cerr <<         "dot_vector_double" << std::endl ;
      std::ofstream stream("dot_vector_double");
      loop< double, dot_vector >( stream, start, step, stop, runs, dot_vector(), dot_functor() );
    } 
    
    {
      std::cerr <<         "dot_vector_range_double" << std::endl ;
      std::ofstream stream("dot_vector_range_double");
      loop< double, dot_vector_range >( stream, start, step, stop, runs, dot_vector_range(), dot_functor() ) ;
    }
    
    {
      std::cerr <<         "dot_vector_slice_double" << std::endl ;
      std::ofstream stream("dot_vector_slice_double");
      loop< double, dot_vector_slice >( stream, start, step, stop, runs, dot_vector_slice(), dot_functor() ) ;
    }
    
    {
      std::cerr <<         "dotu_vector_double_complex" << std::endl ;
      std::ofstream stream("dotu_vector_double_complex");
      loop< std::complex< double >, dot_vector >( stream, start, step, stop, runs, dot_vector(), dotu_functor() );
    }

    {
      std::cerr <<         "dotu_vector_range_double_complex" << std::endl ;
      std::ofstream stream("dotu_vector_range_double_complex");
      loop< std::complex< double >, dot_vector_range >( stream, start, step, stop, runs, dot_vector_range(), dotu_functor() ) ;
    }

    {
      std::cerr <<         "dotu_vector_slice_double_complex" << std::endl ;
      std::ofstream stream("dotu_vector_slice_double_complex");
      loop< std::complex< double >, dot_vector_slice >( stream, start, step, stop, runs, dot_vector_slice(), dotu_functor() ) ;
    }
  }

  return 0 ;
}


