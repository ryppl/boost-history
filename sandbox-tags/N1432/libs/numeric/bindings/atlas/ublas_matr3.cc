
// BLAS level 3

#define USE_STD_VECTOR

#include <iostream>
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

#ifndef USE_STD_VECTOR
typedef ublas::matrix<double, ublas::row_major> m_t;
#else
typedef ublas::matrix<double, ublas::column_major, std::vector<double> > m_t;
#endif 

int main() {

  cout << endl; 

  m_t a (2, 2);
  a (0, 0) = 1.;
  a (0, 1) = 2.;
  a (1, 0) = 3.;
  a (1, 1) = 4.;
  cout << a << endl;
  cout << endl; 

  m_t b (2, 3);
  b (0, 0) = 1.;
  b (0, 1) = 2.;
  b (0, 2) = 3.;
  b (1, 0) = 1.;
  b (1, 1) = 2.;
  b (1, 2) = 3.;
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

  c (0, 0) = 1.;
  c (0, 1) = 1.;
  c (0, 2) = 1.;
  c (1, 0) = 1.;
  c (1, 1) = 1.;
  c (1, 2) = 1.;

  atlas::gemm (2.0, a, b, 0.05, c);
  cout << c << endl;
  cout << endl;

  m_t d (3, 2);
  d (0, 0) = 0.;
  d (0, 1) = 0.;
  d (1, 0) = 0.;
  d (1, 1) = 0.;
  d (2, 0) = 0.;
  d (2, 1) = 0.;

  atlas::gemm (CblasTrans, CblasTrans, 1.0, b, a, 0.0, d);
  cout << d << endl;
  cout << endl; 

  atlas::gemm (CblasTrans, CblasNoTrans, 1.0, a, b, 0.0, c); 
  cout << c << endl;
  cout << endl; 

  atlas::gemm (CblasTrans, CblasNoTrans, 1.0, b, a, 0.0, d);
  cout << d << endl;
  cout << endl; 

  d (0, 0) = 0.;
  d (0, 1) = 0.;
  d (1, 0) = 0.;
  d (1, 1) = 0.;
  d (2, 0) = 0.;
  d (2, 1) = 0.;

  ublas::matrix_range<m_t> br (b, ublas::range (0, 2), ublas::range (1, 3)); 
  ublas::matrix_range<m_t> dr (d, ublas::range (0, 2), ublas::range (0, 2)); 
  
  atlas::gemm (a, br, dr); 
  cout << d << endl;
  cout << endl; 
  
  atlas::gemm (CblasTrans, CblasTrans, 1.0, br, a, 0.0, dr);
  cout << d << endl;
  cout << endl; 

  cout << endl; 

}
