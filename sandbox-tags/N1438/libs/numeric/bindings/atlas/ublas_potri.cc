
// inverting symmetric/hermitian positive definite
// factor (potrf()) and invert (potri())

//#define BOOST_UBLAS_STRICT_HERMITIAN
//  doesn't work (yet?)  

#include <cstddef>
#include <iostream>
#include <complex>
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

int main() {

  cout << endl; 

  // symmetric 
  size_t n = 5; 
  m_t a (n, n);
  symm_t sa (a); 

  for (size_t i = 0; i < n; ++i) {
    sa (i, i) = n;
    for (size_t j = i + 1; j < n; ++j) 
      sa (i, j) = n - (j - i);
  } 

  m_t aa (sa);   // full symmetric matrix (for gemm())

  print_m (a, "A"); 
  cout << "SA:\n";
  cout << sa << endl << endl; 
  print_m (aa, "SA"); 

  int ierr = atlas::potrf (sa); 
  if (!ierr) {
    atlas::potri (sa); 
    m_t ainv (sa); 
    m_t ri (n, n); 
    atlas::gemm (aa, ainv, ri); 
    print_m (ri, "A * Ainv"); 
    atlas::gemm (ainv, aa, ri); 
    print_m (ri, "Ainv * A"); 
  }

  cout << "\n===========================\n" << endl; 

  // hermitian 
  cm_t ca (3, 3); 
  herm_t ha (ca); 

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

  cm_t ca2 (ha);  // full hermitian 
  print_m (ca2, "HA"); 
  
  ierr = atlas::cholesky_factor (ha); 
  if (ierr == 0) {
    atlas::cholesky_invert (ha); 
    cm_t cainv (ha); 
    cm_t ic (3, 3); 
    atlas::gemm (ca2, cainv, ic); 
    print_m (ic, "CA * CAinv"); 
  }
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

  ca2 = ha; 
  print_m (ca2, "HA"); 
  
  ierr = atlas::potrf (ha); 
  if (ierr == 0) {
    atlas::potri (ha); 
    cm_t cainv (ha); 
    cm_t ic (3, 3); 
    atlas::gemm (ca2, cainv, ic); 
    print_m (ic, "CA * CAinv"); 
  }
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

  ca2 = ha; 
  print_m (ca2, "HA"); 
  
  ierr = atlas::potrf (ha); 
  if (ierr == 0) {
    atlas::potri (ha); 
    cm_t cainv (ha); 
    cm_t ic (3, 3); 
    atlas::gemm (ca2, cainv, ic); 
    print_m (ic, "CA * CAinv"); 
  }
  else 
    cout << "matrix is not positive definite: ierr = " 
         << ierr << endl << endl; 

  cout << endl; 

}

