
// solving A * X = B
// in two steps -- factor (getrf()) and solve (getrs())

#include <cstddef>
#include <iostream>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/bindings/atlas/clapack.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/std_vector.hpp>
#include "utils.h"

namespace ublas = boost::numeric::ublas;
namespace atlas = boost::numeric::bindings::atlas;

using std::size_t; 
using std::cout;
using std::endl; 

#ifndef ROW_MAJOR
typedef ublas::matrix<double, ublas::column_major> m_t;
#else
typedef ublas::matrix<double, ublas::row_major> m_t;
#endif

// see leading comments for `gesv()' in clapack.cpp: 
typedef ublas::matrix<double, ublas::column_major> rhs_t;

int main() {

  cout << endl; 

  size_t n = 5; 
  size_t nrhs = 2; 
  m_t a (n, n);
  rhs_t b (n, nrhs);

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
  rhs_t bb (b); 

  ublas::matrix_row<m_t> ar1 (a, 1), ar2 (a, 2);
  ublas::matrix_row<rhs_t> br1 (b, 1), br2 (b, 2);
  swap (ar1, ar2);
  swap (br1, br2);
  print_m (a, "A"); 
  print_m (b, "B"); 
  
  std::vector<int> ipiv (n); 

  atlas::getrf (a, ipiv); 
  atlas::getrs (a, ipiv, b);
  print_m (b, "X"); 

  cout << endl; 

}

