
// BLAS level 2
// TNT 2D array

#include <iostream>
#include <boost/numeric/bindings/atlas/cblas1.hpp>
#include <boost/numeric/bindings/atlas/cblas2.hpp>
#include <boost/numeric/bindings/traits/tnt.hpp>
#include <tnt/tnt.h>

namespace atlas = boost::numeric::bindings::atlas;

using std::cout;
using std::endl; 

#ifndef FORTRAN
typedef TNT::Array1D<double> vct_t;
typedef TNT::Array2D<double> m_t;
#else
typedef TNT::Fortran_Array1D<double> vct_t;
typedef TNT::Fortran_Array2D<double> m_t;
#endif 

int main() {

  cout << endl; 

  vct_t v (2);
  atlas::set (1., v);
  cout << v << endl; 
  vct_t vy (4); 
  atlas::set (0., vy); 
  cout << vy << endl; 
  cout << endl; 

  m_t m (3, 2);
#ifndef FORTRAN
  m[0][0] = 1.;
  m[0][1] = 2.;
  m[1][0] = 3.;
  m[1][1] = 4.;
  m[2][0] = 5.;
  m[2][1] = 6.;
#else
  m (1, 1) = 1.;
  m (1, 2) = 2.;
  m (2, 1) = 3.;
  m (2, 2) = 4.;
  m (3, 1) = 5.;
  m (3, 2) = 6.;
#endif
  cout << m << endl;
  cout << endl; 

  atlas::gemv (CblasNoTrans, 1.0, m, v, 0.0, vy);
  cout << vy << endl; 

  atlas::set (0, vy); 
  atlas::gemv (m, v, vy);
  cout << vy << endl; 

  atlas::set (0, vy); 
  atlas::gemv (1.0, m, v, 0.0, vy);
  cout << vy << endl; 
  cout << endl; 

  atlas::set (0, v); 
  atlas::set (1, vy); 
  atlas::gemv (CblasTrans, 1.0, m, vy, 0.0, v);
  cout << v << endl; 
  cout << endl; 

  atlas::set (1, vy); 
  atlas::gemv (CblasNoTrans, 1.0, m, v, 1.0, vy);
  cout << vy << endl; 

  atlas::set (1, vy); 
  atlas::gemv (CblasNoTrans, 2.0, m, v, 0.5, vy);
  cout << vy << endl; 
  
  cout << endl; 
}
