
// solving A * X = B
// A symmetric/hermitian positive definite
// driver function posv()

//#define BOOST_UBLAS_STRICT_HERMITIAN
//  doesn't work (yet?)  

#include <cstddef>
#include <iostream>
#include <complex>
#include <boost/numeric/bindings/atlas/cblas1.hpp>
#include <boost/numeric/bindings/atlas/cblas2.hpp>
#include <boost/numeric/bindings/atlas/cblas3.hpp>
#include <boost/numeric/bindings/atlas/clapack.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_symmetric_adaptor.hpp>
#include <boost/numeric/bindings/traits/ublas_hermitian_adaptor.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "utils.h"

namespace ublas = boost::numeric::ublas;
namespace atlas = boost::numeric::bindings::atlas;

using std::size_t; 
using std::cout;
using std::endl; 

typedef std::complex<double> cmplx_t; 

#ifndef ROW_MAJOR
typedef ublas::matrix<double, ublas::column_major> m_t;
typedef ublas::matrix<cmplx_t, ublas::column_major> cm_t;
#else
typedef ublas::matrix<double, ublas::row_major> m_t;
typedef ublas::matrix<cmplx_t, ublas::row_major> cm_t;
#endif

#ifndef UPPER
typedef ublas::symmetric_adaptor<m_t, ublas::lower> symm_t; 
typedef ublas::hermitian_adaptor<cm_t, ublas::lower> herm_t; 
#else
typedef ublas::symmetric_adaptor<m_t, ublas::upper> symm_t; 
typedef ublas::hermitian_adaptor<cm_t, ublas::upper> herm_t; 
#endif 

// see leading comments for `gesv()' in clapack.cpp
typedef ublas::matrix<double, ublas::column_major> rhs_t;
typedef ublas::matrix<cmplx_t, ublas::column_major> crhs_t;

int main() {

  cout << endl; 

  // symmetric 
  size_t n = 5; 
  size_t nrhs = 2; 
  m_t a (n, n);
  symm_t sa (a); 
  rhs_t b (n, nrhs);

  for (size_t i = 0; i < n; ++i) {
    sa (i, i) = n;
    b (i, 0) = 0; 
    for (size_t j = i + 1; j < n; ++j) 
      sa (i, j) = n - (j - i);
    for (size_t j = 0; j < n; ++j)
      b (i, 0) += sa (i, j);
    for (size_t k = 1; k < nrhs; ++k)
      b (i, k) = (k + 1) * b (i, 0); 
  } 

  print_m (a, "A"); 
  // print_m (sa, "SA"); 
  cout << "SA:\n";
  cout << sa << endl << endl; 
  print_m (b, "B"); 

  atlas::cholesky_solve (sa, b); 
  print_m (b, "X"); 

  cout << "\n===========================\n" << endl; 

  // hermitian 
  cm_t ca (3, 3); 
  herm_t ha (ca); 
  crhs_t cb (3, 1), cx (3, 1);

#ifndef UPPER
  ha (0, 0) = cmplx_t (3, 0);
  ha (1, 0) = cmplx_t (2, 0);
  ha (1, 1) = cmplx_t (3, 0);
  ha (2, 0) = cmplx_t (1, 0);
  ha (2, 1) = cmplx_t (2, 0);
  ha (2, 2) = cmplx_t (3, 0);
#else
  ha (0, 0) = cmplx_t (3, 0);
  ha (0, 1) = cmplx_t (2, 0);
  ha (0, 2) = cmplx_t (1, 0);
  ha (1, 1) = cmplx_t (3, 0);
  ha (1, 2) = cmplx_t (2, 0);
  ha (2, 2) = cmplx_t (3, 0);
#endif 

  print_m (ca, "CA"); 
  cout << "HA:\n";
  cout << ha << endl << endl; 

  ublas::matrix_column<crhs_t> cxc0 (cx, 0); 
  atlas::set (cmplx_t (1, 0), cxc0);
  print_m (cx, "CX"); 
  
  cm_t ca2 (ha); 
  print_m (ca2, "CA2"); 
  atlas::gemm (ca2, cx, cb); 
  print_m (cb, "CB"); 
  
  int ierr = atlas::posv (ha, cb); 
  if (ierr == 0)
    print_m (cb, "CX"); 
  else 
    cout << "matrix is not positive definite: ierr = " 
         << ierr << endl << endl; 

  cout << "\n===========================\n" << endl; 

#ifndef UPPER
  ha (0, 0) = cmplx_t (3, 0);
  ha (1, 0) = cmplx_t (4, -2);
  ha (1, 1) = cmplx_t (5, 0);
  ha (2, 0) = cmplx_t (-7, -5);
  ha (2, 1) = cmplx_t (0, 3);
  ha (2, 2) = cmplx_t (2, 0);
#else
  ha (0, 0) = cmplx_t (3, 0);
  ha (0, 1) = cmplx_t (4, 2);
  ha (0, 2) = cmplx_t (-7, 5);
  ha (1, 1) = cmplx_t (5, 0);
  ha (1, 2) = cmplx_t (0, -3);
  ha (2, 2) = cmplx_t (2, 0);
#endif

  print_m (ca, "CA"); 
  cout << "HA:\n";
  cout << ha << endl << endl; 

  atlas::set (cmplx_t (1, 0), cxc0);
  print_m (cx, "CX"); 
  
  ca2 = ha; 
  print_m (ca2, "CA2"); 
  atlas::gemm (ca2, cx, cb); 
  print_m (cb, "CB"); 
  
  ierr = atlas::cholesky_solve (ha, cb); 
  if (ierr == 0)
    print_m (cb, "CX"); 
  else 
    cout << "matrix is not positive definite: ierr = " 
         << ierr << endl << endl; 

  cout << "\n===========================\n" << endl; 

#ifndef UPPER
  ha (0, 0) = cmplx_t (25, 0);
  ha (1, 0) = cmplx_t (-5, 5);
  ha (1, 1) = cmplx_t (51, 0);
  ha (2, 0) = cmplx_t (10, -5);
  ha (2, 1) = cmplx_t (4, 6);
  ha (2, 2) = cmplx_t (71, 0);
#else
  ha (0, 0) = cmplx_t (25, 0);
  ha (0, 1) = cmplx_t (-5, -5);
  ha (0, 2) = cmplx_t (10, 5);
  ha (1, 1) = cmplx_t (51, 0);
  ha (1, 2) = cmplx_t (4, -6);
  ha (2, 2) = cmplx_t (71, 0);
#endif

  print_m (ca, "CA"); 
  cout << "HA:\n";
  cout << ha << endl << endl; 

  crhs_t cb33 (3, 3), cx32 (3, 2);
  ublas::matrix_column<crhs_t> cx32c0 (cx32, 0); 
  ublas::matrix_column<crhs_t> cb33c0 (cb33, 0); 
  atlas::set (cmplx_t (1, -1), cx32c0);

  ca2 = ha; 
  print_m (ca2, "CA2"); 
  atlas::gemv (ca2, cx32c0, cb33c0); 
  cb33 (0, 1) = cmplx_t (60, -55);
  cb33 (1, 1) = cmplx_t (34, 58);
  cb33 (2, 1) = cmplx_t (13, -152);
  cb33 (0, 2) = cmplx_t (70, 10);
  cb33 (1, 2) = cmplx_t (-51, 110);
  cb33 (2, 2) = cmplx_t (75, 63);
  print_m (cb33, "CB"); 
  
  ierr = atlas::posv (ha, cb33); 
  if (ierr == 0)
    print_m (cb33, "CX"); 
  else 
    cout << "matrix is not positive definite: ierr = " 
         << ierr << endl << endl; 

  cout << endl; 

}

