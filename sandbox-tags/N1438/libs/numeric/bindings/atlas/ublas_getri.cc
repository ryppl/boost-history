
// inverting A
// using getrf() & getri() 

#include <cstddef>
#include <iostream>
#include <complex>
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

int main() {

  cout << endl; 

  // real
  size_t n = 5; 
  m_t a (n, n);

  for (size_t i = 0; i < n; ++i) {
    a (i, i) = n;
    for (size_t j = i + 1; j < n; ++j)
      a (i, j) = a (j, i) = n - (j - i);
  } 
  print_m (a, "A"); 
  cout << endl; 

  m_t aa (a); 

  std::vector<int> ipiv (n); 
  atlas::lu_factor (a, ipiv);  // getrf()
  atlas::lu_invert (a, ipiv);  // getri() 

  m_t i1 (n, n), i2 (n, n); 
  atlas::gemm (a, aa, i1);
  atlas::gemm (aa, a, i2);

  print_m (i1, "Ainv * A"); 
  print_m (i2, "A * Ainv"); 
  
  cout << endl; 


  // complex 
  cm_t ca (3, 3); 

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

  cm_t caa (ca); 
  
  std::vector<int> ipiv2 (3); 
  
  int ierr = atlas::getrf (ca, ipiv2);
  if (ierr == 0) {
    atlas::getri (ca, ipiv2); 
    cm_t ii (3, 3); 
    atlas::gemm (ca, caa, ii);
    print_m (ii, "CAinv * CA"); 
    atlas::gemm (caa, ca, ii);
    print_m (ii, "CA * CAinv"); 
  }
  else
    cout << "matrix is singular" << endl; 
  
  cout << endl; 

}

