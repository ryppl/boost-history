
// solving A * X = B
// using driver function gesv()

#include <cstddef>
#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/bindings/atlas/clapack.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
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

int main() {

  cout << endl; 

  size_t n = 5; 
  size_t nrhs = 2; 
  m_t a (n, n);
  m_t b (n, nrhs);

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
  m_t const aa (a); 

#ifndef ROW_MAJOR
  m_t bb (b); 
#else 
  // see leading comments for `gesv()' in clapack.cpp
  m_t bb (trans (b)); 
#endif 
  print_m (bb, "B"); 

  atlas::gesv (a, bb); 

#ifndef ROW_MAJOR
  print_m (bb, "X");
  print_m (prod (aa, bb), "B"); 
#else
  print_m (trans (bb), "X"); 
  print_m (prod (aa, trans (bb)), "X"); 
#endif 

  cout << endl; 

}

