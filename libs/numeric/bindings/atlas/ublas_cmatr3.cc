
// BLAS level 3 -- complex numbers

//#define USE_STD_VECTOR

#include <iostream>
#include <complex>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/bindings/atlas/cblas3.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#ifdef USE_STD_VECTOR
#include <vector>
#include <boost/numeric/bindings/traits/std_vector.hpp> 
#endif 

namespace ublas = boost::numeric::ublas;
namespace atlas = boost::numeric::bindings::atlas;

using std::cout;
using std::endl; 

typedef double real_t;
typedef std::complex<real_t> cmplx_t; 

#ifndef USE_STD_VECTOR
typedef ublas::matrix<cmplx_t, ublas::row_major> m_t;
#else
typedef ublas::matrix<cmplx_t, ublas::column_major, std::vector<cmplx_t> > m_t;
#endif 

int main() {

  cout << endl; 

  m_t a (2, 2);
  a (0, 0) = cmplx_t (1., 0.);
  a (0, 1) = cmplx_t (2., 0.);
  a (1, 0) = cmplx_t (3., 0.);
  a (1, 1) = cmplx_t (4., 0.);
  cout << a << endl;
  cout << endl; 

  m_t b (2, 3);
  b (0, 0) = cmplx_t (1., 0.);
  b (0, 1) = cmplx_t (2., 0.);
  b (0, 2) = cmplx_t (3., 0.);
  b (1, 0) = cmplx_t (1., 0.);
  b (1, 1) = cmplx_t (2., 0.);
  b (1, 2) = cmplx_t (3., 0.);
  cout << b << endl;
  cout << endl; 
  
  m_t c (2, 3);
  c (0, 0) = 0.;
  c (0, 1) = 0.;
  c (0, 2) = 0.;
  c (1, 0) = 0.;
  c (1, 1) = 0.;
  c (1, 2) = 0.;
  cout << c << endl;
  cout << endl; 

  atlas::gemm (a, b, c); 
  cout << c << endl;
  atlas::gemm (CblasNoTrans, CblasNoTrans, 1.0, a, b, 0.0, c); 
  cout << c << endl;
  atlas::gemm (1.0, a, b, 0.0, c); 
  cout << c << endl;

  cout << endl; 

}
