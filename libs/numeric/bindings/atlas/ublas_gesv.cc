
// solving A * X = B
// using driver function gesv()

#include <cstddef>
#include <iostream>
#include <complex>
#include <boost/numeric/bindings/atlas/cblas1.hpp>
#include <boost/numeric/bindings/atlas/cblas3.hpp>
#include <boost/numeric/bindings/atlas/clapack.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/std_vector.hpp>
#include "utils.h"

namespace ublas = boost::numeric::ublas;
namespace atlas = boost::numeric::bindings::atlas;

using std::size_t; 
using std::cout;
using std::endl; 

typedef std::complex<double> cmpx; 

#ifndef ROW_MAJOR
typedef ublas::matrix<double, ublas::column_major> m_t;
typedef ublas::matrix<cmpx, ublas::column_major> cm_t;
#else
typedef ublas::matrix<double, ublas::row_major> m_t;
typedef ublas::matrix<cmpx, ublas::row_major> cm_t;
#endif

// see leading comments for `gesv()' in clapack.cpp
typedef ublas::matrix<double, ublas::column_major> rhs_t;
typedef ublas::matrix<cmpx, ublas::column_major> crhs_t;

int main() {

  cout << endl; 

  // real
  size_t n = 5; 
  size_t nrhs = 2; 
  m_t a (n, n);
  rhs_t b (n, nrhs);
  std::vector<int> ipiv (n); 

  for (size_t i = 0; i < n; ++i) {
    a (i, i) = n;
    b (i, 0) = 0; 
    for (size_t j = i + 1; j < n; ++j)
      a (i, j) = a (j, i) = n - (j - i);
    for (size_t j = 0; j < n; ++j)
      b (i, 0) += a (i, j);
    for (size_t k = 1; k < nrhs; ++k)
      b (i, k) = (k + 1) * b (i, 0); 
  } 

  print_m (a, "A"); 
  print_m (b, "B"); 
  m_t aa (a); 
  rhs_t bb (b); 

  atlas::gesv (a, ipiv, b); 
  print_m (b, "X"); 

  atlas::gesv (aa, bb); 
  print_m (bb, "X"); 

  cout << endl; 

  // complex 
  cm_t ca (3, 3); 
  crhs_t cb (3, 1), cx (3, 1);

  ca (0, 0) = cmpx (3, 0);
  ca (0, 1) = cmpx (4, 2);
  ca (0, 2) = cmpx (-7, 5);
  ca (1, 0) = cmpx (4, -2);
  ca (1, 1) = cmpx (-5, 0);
  ca (1, 2) = cmpx (0, -3);
  ca (2, 0) = cmpx (-7, -5);
  ca (2, 1) = cmpx (0, 3);
  ca (2, 2) = cmpx (2, 0);
  print_m (ca, "CA"); 

  ublas::matrix_column<crhs_t> cxc0 (cx, 0); 
  atlas::set (cmpx (1, -1), cxc0);
  atlas::gemm (ca, cx, cb); 
  print_m (cb, "CB"); 
  
  int ierr = atlas::gesv (ca, cb);
  if (ierr == 0) 
    print_m (cb, "CX");
  else
    cout << "matrix is singular" << endl; 

  cout << endl; 

}

