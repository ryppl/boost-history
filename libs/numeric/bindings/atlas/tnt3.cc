
// BLAS level 3
// TNT 2D array

#include <iostream>
#include <boost/numeric/bindings/atlas/cblas3.hpp>
#include <boost/numeric/bindings/traits/tnt.hpp>
#include <tnt/tnt.h>

namespace atlas = boost::numeric::bindings::atlas;

using std::cout;
using std::endl; 

#ifndef FORTRAN
typedef TNT::Array2D<double> m_t;
#else
typedef TNT::Fortran_Array2D<double> m_t;
#endif 

int main() {

  cout << endl; 

  m_t a (2, 2);
#ifndef FORTRAN
  a[0][0] = 1.;
  a[0][1] = 2.;
  a[1][0] = 3.;
  a[1][1] = 4.;
#else
  a (1, 1) = 1.;
  a (1, 2) = 2.;
  a (2, 1) = 3.;
  a (2, 2) = 4.;
#endif
  cout << a << endl;
  cout << endl; 

  m_t b (2, 3);
#ifndef FORTRAN
  b[0][0] = 1.;
  b[0][1] = 2.;
  b[0][2] = 3.;
  b[1][0] = 1.;
  b[1][1] = 2.;
  b[1][2] = 3.;
#else
  b (1, 1) = 1.;
  b (1, 2) = 2.;
  b (1, 3) = 3.;
  b (2, 1) = 1.;
  b (2, 2) = 2.;
  b (2, 3) = 3.;
#endif
  cout << b << endl;
  cout << endl; 

  
  m_t c (2, 3);

#ifndef FORTRAN
  c[0][0] = 0.;
  c[0][1] = 0.;
  c[0][2] = 0.;
  c[1][0] = 0.;
  c[1][1] = 0.;
  c[1][2] = 0.;
#else
  c (1, 1) = 0.;
  c (1, 2) = 0.;
  c (1, 3) = 0.;
  c (2, 1) = 0.;
  c (2, 2) = 0.;
  c (2, 3) = 0.;
#endif

  cout << c << endl;
  cout << endl; 

  atlas::gemm (a, b, c); 
  cout << c << endl;
  cout << endl; 

  atlas::gemm (CblasNoTrans, CblasNoTrans, 1.0, a, b, 0.0, c); 
  cout << c << endl;
  cout << endl; 

  atlas::gemm (1.0, a, b, 0.0, c); 
  cout << c << endl;
  cout << endl; 
  cout << endl; 

#ifndef FORTRAN
  c[0][0] = 1.;
  c[0][1] = 1.;
  c[0][2] = 1.;
  c[1][0] = 1.;
  c[1][1] = 1.;
  c[1][2] = 1.;
#else
  c (1, 1) = 1.;
  c (1, 2) = 1.;
  c (1, 3) = 1.;
  c (2, 1) = 1.;
  c (2, 2) = 1.;
  c (2, 3) = 1.;
#endif

  atlas::gemm (1.0, a, b, 0.05, c);
  cout << c << endl;
  cout << endl;

  m_t d (3, 2);

#ifndef FORTRAN
  d[0][0] = 0.;
  d[0][1] = 0.;
  d[1][0] = 0.;
  d[1][1] = 0.;
  d[2][0] = 0.;
  d[2][1] = 0.;
#else
  d (1, 1) = 0.;
  d (1, 2) = 0.;
  d (2, 1) = 0.;
  d (2, 2) = 0.;
  d (3, 1) = 0.;
  d (3, 2) =0.;
#endif

  cout << d << endl;
  cout << endl; 

  atlas::gemm (CblasTrans, CblasTrans, 1.0, b, a, 0.0, d);
  cout << d << endl;
  cout << endl; 

  atlas::gemm (CblasTrans, CblasNoTrans, 1.0, a, b, 0.0, c); 
  cout << c << endl;
  cout << endl; 

  atlas::gemm (CblasTrans, CblasNoTrans, 1.0, b, a, 0.0, d);
  cout << d << endl;
  cout << endl; 

  cout << endl; 

}
