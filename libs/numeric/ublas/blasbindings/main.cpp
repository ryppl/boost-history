#ifdef BOOST_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>
#include <string>

#include <boost/numeric/ublas/config.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/blas.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <boost/timer.hpp>

// #include "main.hpp"

using namespace boost ;

template<class T>
struct report_mflops {
    void operator () (int multiplies, int plus, int runs, double elapsed) {
        std::cout << (multiplies * numeric::ublas::type_traits<T>::multiplies_complexity + plus * numeric::ublas::type_traits<T>::plus_complexity) * runs / (1024 * 1024 * elapsed) ;
        std::cout << "\t";
        std::cout.flush();
    }
};

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
            report_mflops<value_type> () (0, 1, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
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
            report_mflops<value_type> () (0, 1, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};

template<class T>
struct peak {
  void operator () (int runs) {
    std::cout << "peak performance plus : ";
    peak_c_plus<T> () (runs);
    std::cout << std::endl;
    std::cout << "peak performance multiplies : ";
    peak_c_multiplies<T> () (runs);
    std::cout << std::endl;
  }
};

double norm(double v)
{ return fabs( v ); }

template < typename matrix_type_1, typename matrix_type_2 >
bool equal(const matrix_type_1& m1, const matrix_type_2& m2, double tolerance = 1e-3)
{
  size_t num_rows = m1.size1();
  size_t num_cols = m1.size2();
  double max_diff = 0.0 ;
  for(size_t i = 0; i < num_rows ; ++i )
    for(size_t j = 0; j < num_cols; ++j )
      max_diff = std::max( max_diff, norm(m1(i,j) - m2(i,j)) );
  return max_diff < tolerance ;
}

template < typename matrix_type >
void initialize(matrix_type& m)
{
  for(size_t i = 0 ; i < m.size1() ; ++i )
    for(size_t j = 0 ; j < m.size2() ; ++j )
      m(i,j) = i + j;
}

template < typename T >
void run_tests()
{
  std::cerr << "size vs MFLOPS : prod<row_major>\tprod<column_major>\tgemm" << std::endl;
  boost::timer t;
  for(size_t i = 2; i < 10000 ; ++i ) {
    int runs = std::max<int>(1, 500000 - i*i*i );
    std::cout << i << "\t";
    // row_major
    typedef numeric::ublas::matrix< T, numeric::ublas::row_major > matrix_row_type ;
    matrix_row_type m1_rm(i,i), m2_rm(i,i), result_rm(i,i);
    initialize( m1_rm );
    initialize( m2_rm );
    
    t.restart();
    for(int run_i = 0; run_i < runs ; ++run_i ) result_rm.assign( prod( m1_rm, m2_rm ) );
    report_mflops<T> () (i * i * i, 0, runs, t.elapsed ());

    // column_major 
    typedef numeric::ublas::matrix< T, numeric::ublas::column_major > matrix_column_type ;
    matrix_column_type m1_cm = m1_rm, m2_cm = m2_rm, result_cm(i,i);

    t.restart();
    for(int run_i = 0; run_i < runs ; ++run_i ) result_cm.assign( prod( m1_cm, m2_cm ) );
    report_mflops<T> () (i * i * i, 0, runs, t.elapsed ());

    // BLAS
    T zero = 0.0, one = 1.0;
    matrix_column_type m1_blas = m1_rm, m2_blas = m2_rm, result_blas(i,i);

    t.restart();
    for(int run_i = 0; run_i < runs; ++run_i ) numeric::ublas::gemm( one, m1_blas, m2_blas, zero, result_blas );
    report_mflops<T> () (i * i * i, 0, runs, t.elapsed ());

    if ( ! ( equal( result_rm, result_cm ) && equal( result_rm, result_blas ) ) ) { 
      std::cerr << "regression test failed" << std::endl; 
      std::cerr << result_rm << "\n" << result_cm << "\n" << result_blas << std::endl;
      abort();
    }
    std::cout << std::endl;
  }
}

int main (int argc, char *argv []) {
  //
  // report peak performance of virious datatypes
  std::cerr << "peak float:\n";
  peak<float> () (100000000);

  std::cerr << "peak double:\n";
  peak<double> () (100000000);

  std::cerr << "peak std:complex<float> :\n";
  peak<std::complex<float> > () (100000000);

  std::cerr << "peak std:complex<double> :\n";
  peak<std::complex<double> > () (100000000);

  std::cout << "value_type == double" << std::endl;
  run_tests< double >();

  std::cout << "value_type == std::complex<double>" << std::endl;
  run_tests< std::complex<double> >();

  return 0;
}

