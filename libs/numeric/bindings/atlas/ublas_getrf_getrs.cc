
// solving A * X = B
// in two steps -- factor (getrf()) and solve (getrs())

#include <cstddef>
#include <iostream>
#include <boost/numeric/bindings/atlas/cblas.hpp>
#include <boost/numeric/bindings/atlas/clapack.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/std_vector.hpp>
#include "utils.h"

namespace ublas = boost::numeric::ublas;
namespace atlas = boost::numeric::bindings::atlas;

using std::size_t; 
using std::cout;
using std::endl; 

#ifndef F_ROW_MAJOR
typedef ublas::matrix<double, ublas::column_major> m_t;
#else
typedef ublas::matrix<double, ublas::row_major> m_t;
#endif

// see leading comments for `gesv()' in clapack.hpp: 
typedef ublas::matrix<double, ublas::column_major> rhs_t;

int main() {

  cout << endl; 

  size_t n = 5; 
  m_t a (n, n);   // system matrix 

  size_t nrhs = 2; 
  rhs_t x (n, nrhs), b (n, nrhs);  // b -- right-hand side matrix

  init_symm (a); 
  //     [n   n-1 n-2  ... 1]
  //     [n-1 n   n-1  ... 2]
  // a = [n-2 n-1 n    ... 3]
  //     [        ...       ]
  //     [1   2   ...  n-1 n]

  ublas::matrix_column<rhs_t> xc0 (x, 0), xc1 (x, 1); 
  atlas::set (1., xc0);  // x[.,0] = 1
  atlas::set (2., xc1);  // x[.,1] = 2
#ifndef F_ROW_MAJOR
  atlas::gemm (a, x, b);  // b = a x, so we know the result ;o) 
#else
  // a is row major, b & x are column major and we can't use gemm()
  ublas::matrix_column<rhs_t> bc0 (b, 0), bc1 (b, 1); 
  atlas::gemv (a, xc0, bc0);  // b[.,0] = a x[.,0]
  atlas::gemv (a, xc1, bc1);  // b[.,1] = a x[.,1]  =>  b = a x
#endif 

  print_m (a, "A"); 
  cout << endl; 
  print_m (b, "B"); 
  cout << endl; 

  ublas::matrix_row<m_t> ar1 (a, 1), ar3 (a, 3);
  ublas::matrix_row<rhs_t> br1 (b, 1), br3 (b, 3);
  swap (ar1, ar3);   // swap rows to force pivoting 
  swap (br1, br3);
  print_m (a, "A");  // print `new' system  
  cout << endl; 
  print_m (b, "B"); 
  cout << endl; 
  
  std::vector<int> ipiv (n);  // pivot vector

  atlas::getrf (a, ipiv);      // factor a
  atlas::getrs (a, ipiv, b);   // solve from factorization 
  print_m (b, "X"); 
  cout << endl; 

  print_v (ipiv, "pivots"); 

  cout << endl; 

}

