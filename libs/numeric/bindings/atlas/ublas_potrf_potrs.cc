
// solving A * X = B
// A symmetric/hermitian positive definite
// factor (potrf()) and solve (potrs())

// #define BOOST_UBLAS_STRICT_HERMITIAN
// .. doesn't work (yet?)  

#include <cstddef>
#include <iostream>
#include <complex>
#include <boost/numeric/bindings/atlas/cblas1.hpp>
#include <boost/numeric/bindings/atlas/cblas2.hpp>
#include <boost/numeric/bindings/atlas/cblas3.hpp>
#include <boost/numeric/bindings/atlas/clapack.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_symmetric.hpp>
#include <boost/numeric/bindings/traits/ublas_hermitian.hpp>
#include "utils.h"

namespace ublas = boost::numeric::ublas;
namespace atlas = boost::numeric::bindings::atlas;

using std::size_t; 
using std::cout;
using std::endl; 

typedef std::complex<double> cmplx_t; 

#ifndef F_ROW_MAJOR
typedef ublas::matrix<double, ublas::column_major> m_t;
typedef ublas::matrix<cmplx_t, ublas::column_major> cm_t;
#else
typedef ublas::matrix<double, ublas::row_major> m_t;
typedef ublas::matrix<cmplx_t, ublas::row_major> cm_t;
#endif

#ifndef F_UPPER
typedef ublas::symmetric_adaptor<m_t, ublas::lower> symm_t; 
typedef ublas::hermitian_adaptor<cm_t, ublas::lower> herm_t; 
#else
typedef ublas::symmetric_adaptor<m_t, ublas::upper> symm_t; 
typedef ublas::hermitian_adaptor<cm_t, ublas::upper> herm_t; 
#endif 

// see leading comments for `gesv()' in clapack.hpp
typedef ublas::matrix<double, ublas::column_major> rhs_t;
typedef ublas::matrix<cmplx_t, ublas::column_major> crhs_t;

int main() {

  // for more descriptive comments see ublas_posv.cc 
  cout << endl; 

  // symmetric 
  cout << "real symmetric\n" << endl; 

  size_t n = 5; 
  size_t nrhs = 2; 
  m_t a (n, n);    
  symm_t sa (a);   
  rhs_t x (n, nrhs), b (n, nrhs);  

  init_symm (sa, 'l'); 
  print_m (sa, "sa"); 
  cout << endl; 

  ublas::matrix_column<rhs_t> xc0 (x, 0), xc1 (x, 1); 
  atlas::set (1., xc0);  
  atlas::set (2., xc1);  
#ifndef F_ROW_MAJOR
  atlas::symm (sa, x, b);
#else
  ublas::matrix_column<rhs_t> bc0 (b, 0), bc1 (b, 1); 
  atlas::symv (sa, xc0, bc0); 
  atlas::symv (sa, xc1, bc1); 
#endif 
  print_m (b, "b"); 
  cout << endl; 

  int ierr = atlas::cholesky_factor (sa);  // potrf()
  if (!ierr) {
    atlas::cholesky_triang_solve (sa, b);  // potrs() 
    print_m (b, "x"); 
  }
  cout << endl; 

  /////////////////////////////////////////////////////////
  // hermitian 
  cout << "\n===========================\n" << endl; 
  cout << "complex hermitian\n" << endl; 

  cm_t ca (3, 3);  
  herm_t ha (ca);  
  crhs_t cb (3, 1), cx (3, 1);

  // regular, but not positive definite 
#ifndef F_UPPER
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
  print_m (ha, "ha"); 
  cout << endl; 

  ublas::matrix_column<crhs_t> cxc0 (cx, 0), cbc0 (cb, 0);
  atlas::set (cmplx_t (1, 1), cxc0);
  print_m (cx, "cx"); 
  cout << endl; 
  
  atlas::hemv (ha, cxc0, cbc0); 
  print_m (cb, "cb"); 
  cout << endl; 
  
  ierr = atlas::potrf (ha); 
  if (ierr == 0) {
    atlas::potrs (ha, cb);
    print_m (cb, "cx"); 
  }
  else 
    cout << "matrix is not positive definite: ierr = " 
         << ierr << endl; 
  cout << endl; 

  cout << "\n===========================\n" << endl; 
  cout << "complex hermitian\n" << endl; 

#ifndef F_UPPER
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
  print_m (ha, "ha"); 
  cout << endl; 

  crhs_t cb32 (3, 2); 
  cb32 (0, 0) = cmplx_t (60, -55);
  cb32 (1, 0) = cmplx_t (34, 58);
  cb32 (2, 0) = cmplx_t (13, -152);
  cb32 (0, 1) = cmplx_t (70, 10);
  cb32 (1, 1) = cmplx_t (-51, 110);
  cb32 (2, 1) = cmplx_t (75, 63);
  print_m (cb32, "cb"); 
  cout << endl; 
  
  ierr = atlas::potrf (ha); 
  if (ierr == 0) {
    atlas::potrs (ha, cb32);
    print_m (cb32, "cx"); 
  }
  else 
    cout << "matrix is not positive definite: ierr = " 
         << ierr << endl; 
  cout << endl; 

}

