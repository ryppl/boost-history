
// solving A * X = B
// using driver function gesv()

#include <cstddef>
#include <iostream>
#include <complex>
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

typedef std::complex<double> cmpx; 

#ifndef F_ROW_MAJOR
typedef ublas::matrix<double, ublas::column_major> m_t;
typedef ublas::matrix<cmpx, ublas::column_major> cm_t;
#else
typedef ublas::matrix<double, ublas::row_major> m_t;
typedef ublas::matrix<cmpx, ublas::row_major> cm_t;
#endif

// see leading comments for `gesv()' in clapack.hpp
typedef ublas::matrix<double, ublas::column_major> rhs_t;
typedef ublas::matrix<cmpx, ublas::column_major> crhs_t;

int main() {

  cout << endl; 
  cout << "real system:" << endl << endl; 

  size_t n = 5;   
  m_t a (n, n);   // system matrix 

  size_t nrhs = 2; 
  rhs_t x (n, nrhs), b (n, nrhs);  // b -- right-hand side matrix

  std::vector<int> ipiv (n);  // pivot vector

  init_symm (a); 
  //     [n   n-1 n-2  ... 1]
  //     [n-1 n   n-1  ... 2]
  // a = [n-2 n-1 n    ... 3]
  //     [        ...       ]
  //     [1   2   ...  n-1 n]

  m_t aa (a);  // copy of a, because a is `lost' after gesv()

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

  atlas::gesv (a, ipiv, b);   // solving the system, b contains x 
  print_m (b, "X"); 
  cout << endl; 

#ifndef F_ROW_MAJOR
  atlas::gemm (aa, b, x);     // check the solution 
#else
  atlas::gemv (aa, bc0, xc0);
  atlas::gemv (aa, bc1, xc1);
#endif 
  print_m (x, "B = A X"); 
  cout << endl; 

  ////////////////////////////////////////////////////////

  cout << endl; 
  cout << "complex system:" << endl << endl; 
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
  cout << endl; 

  ublas::matrix_column<crhs_t> cxc0 (cx, 0),  cbc0 (cb, 0); 
  atlas::set (cmpx (1, -1), cxc0);
  atlas::gemv (ca, cxc0, cbc0); 
  print_m (cb, "CB"); 
  cout << endl; 
  
  int ierr = atlas::gesv (ca, cb); // with `internal' pivot vector
  if (ierr == 0) 
    print_m (cb, "CX");
  else
    cout << "matrix is singular" << endl; 

  cout << endl; 

}

