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
#include <boost/numeric/ublas/io.h>

#include <boost/timer.hpp>

#include "main.hpp"

void header (std::string text) {
    std::cout << text << std::endl;
    //std::cerr << text << std::endl;
}
template < typename matrix_type >
bool zero( const matrix_type& m, double tolerance = 1e-8)
{
  bool is_zero = true;
  size_t num_rows = m.size1();
  size_t num_cols = m.size2();
  for(size_t i = 0; i < num_rows ; ++i )
    for(size_t j = 0; j < num_cols; ++j )
      is_zero = is_zero && ( m(i,j) < tolerance ) ;
  return is_zero;
}


template<class T>
struct peak_c_plus {
    typedef T value_type;

    void operator () (int runs) const {
        try {
            static T s (0);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                s += T (0);
//                sink_scalar (s);
            }
            footer<value_type> () (0, 1, runs, t.elapsed ());
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
//                sink_scalar (s);
            }
            footer<value_type> () (0, 1, runs, t.elapsed ());
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
void peak<T>::operator () (int runs) {
    header ("peak");

    header ("plus");
    peak_c_plus<T> () (runs);

    header ("multiplies");
    peak_c_multiplies<T> () (runs);
}


template < class matrix_type >
struct bench_ublas_gemm
{
  typedef typename matrix_type::value_type value_type;

  matrix_type operator()(int runs, const matrix_type& m1, const matrix_type& m2) const
  {
    matrix_type result( m1.size1(), m2.size2() );
    boost::timer t;
    for(int i = 0; i < runs ; ++i ) result.assign( prod( m1, m2 ) );
    footer<value_type> () (m1.size1() * m1.size2() * m2.size2(), 0, runs, t.elapsed ());
    return result ;
  }
};

template < class matrix_type >
struct bench_blas_gemm
{
  typedef typename matrix_type::value_type value_type;

  matrix_type operator()(int runs, const matrix_type& m1, const matrix_type& m2) const
  {
    matrix_type result( m1.size1(), m2.size2() );
    value_type zero = 0.0, one = 1.0;
    boost::timer t;
    for(int i = 0; i < runs; ++i ) numerics::gemm( one, m1, m2, zero, result );
    footer<value_type> () (m1.size1() * m1.size2() * m2.size2(), 0, runs, t.elapsed ());
    return result;
  }
};

template < typename matrix_type >
void initialize(matrix_type& m)
{
  for(size_t i = 0 ; i < m.size1() ; ++i )
    for(size_t j = 0 ; j < m.size2() ; ++j )
      m(i,j) = i + j;
}

template <class T, int N, int K, int M>
void bench_gemm(int runs) {
  header("bench_gemm");

  header("row_major");
  typedef numerics::matrix< T, numerics::row_major > matrix_row_type ;
  matrix_row_type m1_rm(N,K), m2_rm(K,M), result_rm( N,M );
  initialize( m1_rm );
  initialize( m2_rm );
  result_rm = bench_ublas_gemm< matrix_row_type >()(runs, m1_rm, m2_rm );
  //std::cout << "result : " << result_rm << std::endl;

  header("column_major");
  typedef numerics::matrix< T, numerics::column_major > matrix_column_type ;
  matrix_column_type m1_cm = m1_rm, m2_cm = m2_rm, result_cm( N,M );
  result_cm = bench_ublas_gemm< matrix_column_type >()(runs, m1_cm, m2_cm);
  //std::cout << "result : " << result_cm << std::endl;

  header("blas");
  matrix_column_type result_blas( N,M );
  result_blas = bench_blas_gemm< matrix_column_type >()( runs, m1_cm, m2_cm );
  //std::cout << "result : " << result_blas << std::endl;

  if ( zero( result_rm - result_cm ) && zero( result_rm - result_blas ) )
    { std::cerr << "regression test succeeded " << std::endl; }
  else
    { std::cerr << "regression test failed" << std::endl; abort() ; }
}

template <class T, int N, int K, int M>
void bench_gemm_n_t(int runs) {
  header("bench_gemm_n_t");

  header("row_major");
  typedef numerics::matrix< T, numerics::row_major > matrix_row_type ;
  matrix_row_type m1_rm(N,K), m2_rm(M,K), result_rm( N,M );
  initialize( m1_rm );
  initialize( m2_rm );
  result_rm = bench_ublas_gemm< matrix_row_type >()(runs, m1_rm, trans( m2_rm ) );
  //std::cout << "result : " << result_rm << std::endl;

  header("column_major");
  typedef numerics::matrix< T, numerics::column_major > matrix_column_type ;
  matrix_column_type m1_cm = m1_rm, m2_cm = m2_rm, result_cm( N,M );
  result_cm = bench_ublas_gemm< matrix_column_type >()(runs, m1_cm, trans( m2_cm ) );
  //std::cout << "result : " << result_cm << std::endl;

  header("blas");
  matrix_column_type result_blas( N,M );
  result_blas = bench_blas_gemm< matrix_column_type >()( runs, m1_cm, trans( m2_cm ) );
  //std::cout << "result : " << result_blas << std::endl;

  if ( zero( result_rm - result_cm ) && zero( result_rm - result_blas ) )
    { std::cerr << "regression test succeeded " << std::endl; }
  else
    { std::cerr << "regression test failed" << std::endl; abort() ; }
}

//template struct peak<float>;
//template struct peak<double>;
//template struct peak<std::complex<float> >;
//template struct peak<std::complex<double> >;

int main (int argc, char *argv []) {
    header ("float");
    peak<float> () (100000000);

    header ("double");
    peak<double> () (100000000);

#ifdef USE_COMPLEX
    header ("std:complex<float>");
    peak<std::complex<float> > () (100000000);

    header ("std:complex<double>");
    peak<std::complex<double> > () (100000000);
#endif

    int scale = 1;
    if (argc > 1)
        scale = atoi (argv [1]);

#ifdef USE_FLOAT
    /*
    header ("float, 3");

    bench_1<float, 3> () (1000000 * scale);
    bench_2<float, 3> () (300000 * scale);
    bench_3<float, 3> () (100000 * scale);

    header ("float, 10");

    bench_1<float, 10> () (300000 * scale);
    bench_2<float, 10> () (30000 * scale);
    bench_3<float, 10> () (3000 * scale);

    header ("float, 30");

    bench_1<float, 30> () (100000 * scale);
    bench_2<float, 30> () (3000 * scale);
    bench_3<float, 30> () (100 * scale);

    header ("float, 100");

    bench_1<float, 100> () (30000 * scale);
    bench_2<float, 100> () (300 * scale);
    bench_3<float, 100> () (3 * scale);
    */
#endif

    header ("\ndouble, 3");

    bench_gemm<double, 3, 3, 3>(1000000 * scale);
    bench_gemm_n_t<double, 3, 3, 3>(1000000 * scale);
    //bench_2<double, 3> () (300000 * scale);
    //bench_3<double, 3> () (100000 * scale);

    header ("\ndouble, 10");

    bench_gemm<double, 10, 10, 10>(300000 * scale);
    bench_gemm_n_t<double, 10, 10, 10>(300000 * scale);
    //bench_2<double, 10> () (30000 * scale);
    //bench_3<double, 10> () (3000 * scale);

    header ("\ndouble, 30");

    bench_gemm<double, 30, 30, 30>(10000 * scale);
    bench_gemm_n_t<double, 30, 30, 30>(10000 * scale);
    //bench_2<double, 30> () (3000 * scale);
    //bench_3<double, 30> () (100 * scale);

    header ("\ndouble, 100");

    bench_gemm<double, 100, 100, 100>(10 * scale);
    bench_gemm_n_t<double, 100, 100, 100>(10 * scale);
    //bench_2<double, 100> () (300 * scale);
    //bench_3<double, 100> () (3 * scale);

    header ("\ndouble, 1000");
    bench_gemm<double, 1000, 100, 1000>(1 * scale);
    bench_gemm_n_t<double, 1000, 100, 1000>(1 * scale);

#ifdef USE_STD_COMPLEX
    /*
    header ("std::complex<float>, 3");

    bench_1<std::complex<float>, 3> () (1000000 * scale);
    bench_2<std::complex<float>, 3> () (300000 * scale);
    bench_3<std::complex<float>, 3> () (100000 * scale);

    header ("std::complex<float>, 10");

    bench_1<std::complex<float>, 10> () (300000 * scale);
    bench_2<std::complex<float>, 10> () (30000 * scale);
    bench_3<std::complex<float>, 10> () (3000 * scale);

    header ("std::complex<float>, 30");

    bench_1<std::complex<float>, 30> () (100000 * scale);
    bench_2<std::complex<float>, 30> () (3000 * scale);
    bench_3<std::complex<float>, 30> () (100 * scale);

    header ("std::complex<float>, 100");

    bench_1<std::complex<float>, 100> () (30000 * scale);
    bench_2<std::complex<float>, 100> () (300 * scale);
    bench_3<std::complex<float>, 100> () (3 * scale);

    header ("std::complex<double>, 3");

    bench_1<std::complex<double>, 3> () (1000000 * scale);
    bench_2<std::complex<double>, 3> () (300000 * scale);
    bench_3<std::complex<double>, 3> () (100000 * scale);

    header ("std::complex<double>, 10");

    bench_1<std::complex<double>, 10> () (300000 * scale);
    bench_2<std::complex<double>, 10> () (30000 * scale);
    bench_3<std::complex<double>, 10> () (3000 * scale);

    header ("std::complex<double>, 30");

    bench_1<std::complex<double>, 30> () (100000 * scale);
    bench_2<std::complex<double>, 30> () (3000 * scale);
    bench_3<std::complex<double>, 30> () (100 * scale);

    header ("std::complex<double>, 100");

    bench_1<std::complex<double>, 100> () (30000 * scale);
    bench_2<std::complex<double>, 100> () (300 * scale);
    bench_3<std::complex<double>, 100> () (3 * scale);
    */
#endif

    return 0;
}

