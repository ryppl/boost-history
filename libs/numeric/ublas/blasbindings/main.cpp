#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/blasbindings.hpp>
#include <boost/timer.hpp>

namespace numerics = boost::numeric::ublas ;

template < typename T >
void random_initialise(T& v)
{ v = 10.0 * rand()/(RAND_MAX+1.0) ; }

template < typename T >
void random_initialise_vector(numerics::vector< T >& v)
{
  size_t size = v.size();
  for(size_t i = 0 ; i < size ; ++i ) random_initialise( v[i] ) ;
}

template < typename T, typename Orientation >
void random_initialise_matrix(numerics::matrix< T, Orientation >& m)
{
  size_t size1 = m.size1();
  size_t size2 = m.size2();
  for(size_t i = 0 ; i < size1 ; ++i ) 
    for(size_t j = 0 ; j < size2 ; ++j )
      random_initialise( m(i,j) ) ;
}

template < typename T >
struct assign_multiplier
{
   T operator()() const
   { return 1.0000002 ; } // otherwise the accumulated result of the range will result in 0.0 or over/under-flow
};

template < typename T >
struct assign_multiplier< std::complex< T > >
{
  std::complex< T > operator()() const
  { return std::complex< T >( cos(0.5),sin(0.5) ) + ( assign_multiplier< T >()() - 1.0 ) ; }
};

template<class T>
double flops(int multiplies, int plus, int runs, double elapsed) 
{ return ( multiplies * boost::numeric::ublas::type_traits<T>::multiplies_complexity + plus * boost::numeric::ublas::type_traits<T>::plus_complexity ) * runs / (1024 * 1024 * elapsed) ; }

template<class T>
struct peak_c_plus {
  typedef T value_type;
  
  void operator () (int runs) const {
    try {
      static T s (0);
      boost::timer t;
      for (int i = 0; i < runs; ++ i) {
        s += T (0);
      }
      std::cerr << flops<value_type>(0, 1, runs, t.elapsed ()) << " Mflops\n";
    }
    catch (std::exception &e) {
      std::cerr << e.what () << std::endl;
    }
    catch (...) {
      std::cerr << "unknown exception" << std::endl;
    }
  }
};

template<class T>
struct peak_c_multiplies {
  typedef T value_type;

  void operator () (int runs) const {
    try {
      static T s (1);
      boost::timer t;
      for (int i = 0; i < runs; ++ i) {
        s *= T (1);
      }
      std::cerr << flops<value_type>(0, 1, runs, t.elapsed ()) << " Mflops\n";
    }
    catch (std::exception &e) {
      std::cerr << e.what () << std::endl;
    }
    catch (...) {
      std::cerr << "unknown exception" << std::endl;
    }
  }
};

template<class T>
struct peak
{
  void operator () (int runs) 
  {
    std::cerr << "plus       :";
    peak_c_plus<T> () (runs);
    std::cerr << "multiplies :";
    peak_c_multiplies<T> () (runs);
  }
};

template < typename value_type >
void check(value_type a, value_type b)
{
  if ( ! eq< value_type >( a, b, 1e-5 ) ) {
    std::cerr << "\n\nregression test failure : results are not identical" << std::endl;
    std::cerr << a << " != " << b << std::endl;
    exit( 1 );  
  }
}

template < typename Iterator0, typename Iterator1 >
void check(Iterator0 begin0, Iterator0 end0, Iterator1 begin1)
{
  Iterator0 it0 = begin0 ;
  Iterator1 end1 = begin1 + std::distance(begin0,end0);
  for(bool fail = false ; it0 != end0 || fail ; ++it0 ) ; // fail = ! ( *it0 < std::numeric_limits< typename Iterator0::value_type >::max() );

  Iterator1 it1 = begin1 ;
  for(bool fail = false ; it1 != end1 || fail ; ++it1 ) ; // fail = ! ( *it1 < std::numeric_limits< typename Iterator0::value_type >::max() ) ;

  if ( it0 != end0 || it1 != end1 ) {
    std::cerr << "\n\nregression test failure : results overflowed" << std::endl;
    std::copy( begin0, end0, std::ostream_iterator< typename Iterator0::value_type >( std::cerr, " " ) ); std::cerr << std::endl;
    std::copy( begin1, end1, std::ostream_iterator< typename Iterator1::value_type >( std::cerr, " " ) ); std::cerr << std::endl;
    exit(1);
  }

  if ( ! std::equal( begin0, end0, begin1, is_equal< typename Iterator0::value_type >( std::abs( *begin0 ) * 1e-5 ) ) ) {
    std::cerr << "\n\nregression test failure : results are not identical" << std::endl;
    std::cerr << std::setprecision( 20 ) ;
    std::copy( begin0, end0, std::ostream_iterator< typename Iterator0::value_type >( std::cerr, " " ) ); std::cerr << std::endl;
    std::copy( begin1, end1, std::ostream_iterator< typename Iterator1::value_type >( std::cerr, " " ) ); std::cerr << std::endl;
    exit( 1 );  
  }
}

template < typename value_type >
void check(numerics::matrix< value_type > &a, numerics::matrix< value_type > &b)
{
  bool ret = true ;
  size_t num_rows = a.size1() ;
  size_t num_cols = a.size2() ;

  for(size_t i = 0 ; i < num_rows && ret ; ++i ) 
    for(size_t j = 0 ; j < num_cols && ret ; ++j ) 
      ret = eq< value_type >( a(i,j), b(i,j), 1e-5 );

  if ( ! ret ) {
    std::cerr << "\n\nregression test failure : matrices not identical" << std::endl;
    exit( 1 );  
  }
}

template < typename T >
void report(std::ostream& os, int runs, int runs_i, int size_i, double time)
{
  double normed_time = runs * time / ( runs_i * size_i ) ;
  double mflops = flops<T>(size_i,0,runs_i,time) ;
  std::cerr << std::setw(12) << normed_time << std::setw(12) << mflops ;
  os        << std::setw(12) << normed_time << std::setw(12) << mflops ;
}

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

template < typename VectorType >
void test_axpy(std::ostream& os, int runs, int runs_i, int size, int size_i, typename VectorType::value_type alpha, VectorType& x_native, VectorType& y_native, VectorType& x_toblas, VectorType& y_toblas)
{
  typedef typename VectorType::value_type value_type ;

  boost::timer t ;
  for(int i = 0 ; i < runs_i ; ++i ) y_native += alpha * x_native ;
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  t.restart() ;
  for(int i = 0 ; i < runs_i ; ++i ) numerics::axpy( alpha, x_toblas, y_toblas );
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  check( y_native.begin(), y_native.end(), y_toblas.begin() ) ;
}

struct axpy_vector
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > x_native( size_i ); 
    random_initialise_vector( x_native );
    numerics::vector< T > y_native( x_native ) ;
    numerics::vector< T > x_toblas( x_native ) ;
    numerics::vector< T > y_toblas( x_native ) ;
    T alpha = std::numeric_limits< T >::epsilon() ;
      
    test_axpy( os, runs, runs_i, size, size_i, alpha, x_native, y_native, x_toblas, y_toblas );
  }
};

struct axpy_vector_range
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > x_native( size ); 
    random_initialise_vector( x_native );
    numerics::vector< T > y_native( x_native ) ;
    numerics::vector< T > x_toblas( x_native ) ;
    numerics::vector< T > y_toblas( x_native ) ;
    T alpha = std::numeric_limits< T >::epsilon() ;

    int start = std::max( 0, ( size / 2 ) - size_i );
    int stop  = start + size_i ;
    numerics::vector_range< numerics::vector< T > > x_native_range( x_native, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > y_native_range( y_native, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > x_toblas_range( x_toblas, numerics::range(start,stop) );
    numerics::vector_range< numerics::vector< T > > y_toblas_range( y_toblas, numerics::range(start,stop) );
      
    test_axpy( os, runs, runs_i, size, size_i, alpha, x_native_range, y_native_range, x_toblas_range, y_toblas_range);
  }
};

struct axpy_vector_slice
{
  template < typename T >
  void operator()(std::ostream& os, int size, int size_i, int runs, int runs_i)
  {
    numerics::vector< T > x_native( size ); 
    random_initialise_vector( x_native );
    numerics::vector< T > y_native( x_native ) ;
    numerics::vector< T > x_toblas( x_native ) ;
    numerics::vector< T > y_toblas( x_native ) ;
    T alpha = std::numeric_limits< T >::epsilon() ;

    int stride = size / size_i ;
    numerics::vector_slice< numerics::vector< T > > x_native_slice( x_native, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > y_native_slice( y_native, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > x_toblas_slice( x_toblas, numerics::slice(0,stride,size_i) );
    numerics::vector_slice< numerics::vector< T > > y_toblas_slice( y_toblas, numerics::slice(0,stride,size_i) );
      
    test_axpy( os, runs, runs_i, size, size_i, alpha, x_native_slice, y_native_slice, x_toblas_slice, y_toblas_slice ) ;
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

template < typename ValueType, typename MatrixType, typename VectorType >
void test_gemv(std::ostream& os, int runs, int runs_i, int size, int size_i, ValueType alpha, ValueType beta, MatrixType &a, VectorType& x, VectorType &y_native, VectorType &y_toblas)
{
  typedef typename VectorType::value_type value_type ;

  boost::timer t ;
  for(int i = 0 ; i < runs_i ; ++i ) y_native += alpha * numerics::prod(a, x) ;
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  t.restart() ;
  for(int i = 0 ; i < runs_i ; ++i ) numerics::gemv( alpha, a, x, beta, y_toblas ) ;
  
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
      
    test_gemv( os, runs, runs_i, size, size_i, alpha, beta, a, x, y_native, y_toblas );
  }
};

template < typename ValueType, typename MatrixType, typename VectorType >
void test_trans_gemv(std::ostream& os, int runs, int runs_i, int size, int size_i, ValueType alpha, ValueType beta, MatrixType &a, VectorType& x, VectorType &y_native, VectorType &y_toblas)
{
  typedef typename VectorType::value_type value_type ;

  boost::timer t ;
  for(int i = 0 ; i < runs_i ; ++i ) y_native += alpha * numerics::prod( numerics::trans(a), x) ;
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  t.restart() ;
  for(int i = 0 ; i < runs_i ; ++i ) numerics::gemv( alpha, numerics::trans(a), x, beta, y_toblas ) ;
  
  report< value_type >( os, runs, runs_i, size_i, t.elapsed() );
  
  check( y_native.begin(), y_native.end(), y_toblas.begin() );
}

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
      
    test_trans_gemv( os, runs, runs_i, size, size_i, alpha, beta, a, x, y_native, y_toblas );
  }
};

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

template < typename T, typename FunctorType >
void loop(std::ostream& os, int start, int step, int stop, int runs, FunctorType functor)
{
  for(int size_i = start ; size_i <= stop ; size_i = std::max( static_cast< int >( size_i * (1 + 1.0 / step ) ), size_i + 1 ) ) {
    int runs_i = 10 * runs / size_i ;
    
    std::cerr << size_i << "\t";
    os        << size_i << "\t";
    
    functor.template operator()< T >( os, stop, size_i, runs, runs_i ) ;
    
    std::cerr << std::endl;
    os        << std::endl;
  }
}

template < typename T, typename FunctorType, typename CallFunctor >
void loop(std::ostream& os, int start, int step, int stop, int runs, FunctorType functor, CallFunctor ublas_call)
{
  for(int size_i = start ; size_i <= stop ; size_i = std::max( static_cast< int >( size_i * (1 + 1.0 / step ) ), size_i + 1 ) ) {
    int runs_i = 10 * runs / size_i ;
    
    std::cerr << size_i << "\t";
    os        << size_i << "\t";
    
    functor.template operator()< T >( os, stop, size_i, runs, runs_i, ublas_call ) ;
    
    std::cerr << std::endl;
    os        << std::endl;
  }
}

void check_ublas()
{
  namespace numerics = boost::numeric::ublas ;
  using namespace boost::numeric::ublas ;
  numerics::vector< double > v(10);
  vector_slice< numerics::vector< double > > s( v, numerics::slice(0,3,3) );
  vector_range< vector_slice< numerics::vector< double > > > rs( s, range(1,3) );
  std::cout << "stride of a vector : " << v.stride() << std::endl;
  std::cout << "stride of a vector_slice_range : " << rs.stride() << std::endl;
}

int main (int argc, char *argv []) 
{
  int runs = 10000000 ;
  int start = 1 ;
  int step  = 50 ;
  int stop  = 10000 ;

  check_ublas() ;

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

  return 0 ;
}


