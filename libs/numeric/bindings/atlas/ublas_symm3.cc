
// BLAS level 3
// symmetric matrix

#include <stddef.h>
#include <iostream>
#include <boost/numeric/bindings/atlas/cblas.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_symmetric.hpp>
#include "utils.h"

namespace ublas = boost::numeric::ublas;
namespace atlas = boost::numeric::bindings::atlas;
namespace traits = boost::numeric::bindings::traits;

using std::cout;
using std::cin;
using std::endl; 

typedef double real_t; 

typedef ublas::matrix<real_t, ublas::column_major> cm_t; 
typedef ublas::matrix<real_t, ublas::row_major> rm_t; 

typedef ublas::symmetric_adaptor<cm_t, ublas::upper> ucsymm_t; 
typedef ublas::symmetric_adaptor<cm_t, ublas::lower> lcsymm_t; 
typedef ublas::symmetric_adaptor<rm_t, ublas::upper> ursymm_t; 
typedef ublas::symmetric_adaptor<rm_t, ublas::lower> lrsymm_t; 

int main() {
  
  cout << endl; 

  size_t n; 
  cout << "n -> "; 
  cin >> n;
  cout << endl; 

  cm_t uc (n, n);
  ucsymm_t ucs (uc); 
  cm_t lc (n, n); 
  lcsymm_t lcs (lc); 
  rm_t ur (n, n); 
  ursymm_t urs (ur); 
  rm_t lr (n, n); 
  lrsymm_t lrs (lr); 

  init_symm (ucs, 'l'); 
  init_symm (lcs, 'l'); 
  init_symm (urs, 'l'); 
  init_symm (lrs, 'l'); 

  print_m (ucs, "ucs");
  cout << endl; 
  print_m_data (ucs, "ucs");
  cout << endl; 

  print_m (lcs, "lcs");
  cout << endl; 
  print_m_data (lcs, "lcs");
  cout << endl; 

  print_m (urs, "urs");
  cout << endl; 
  print_m_data (urs, "urs");
  cout << endl; 

  print_m (lrs, "lrs");
  cout << endl; 
  print_m_data (lrs, "lrs");
  cout << endl; 

#ifndef F_RUNTIME_FAILURE
  cm_t cb (n, n); 
  rm_t rb (n, n); 
  cm_t cc (n, n); 
  rm_t rc (n, n); 
#else
  cm_t cb (n, n+1); 
  rm_t rb (n, n+1); 
  cm_t cc (n, n+1); 
  rm_t rc (n, n+1); 
#endif 

  init_m (cb, rws1());
  init_m (rb, rws1());

  print_m (cb, "cb");
  cout << endl; 
  print_m (rb, "rb");
  cout << endl; 

  atlas::symm (ucs, cb, cc); 
  print_m (cc, "c = a b");
  cout << endl; 

  atlas::symm (lcs, cb, cc);  
  print_m (cc, "c = a b");
  cout << endl; 

  atlas::symm (urs, rb, rc); 
  print_m (rc, "c = a b");
  cout << endl; 

  atlas::symm (lrs, rb, rc); 
  print_m (rc, "c = a b");
  cout << endl; 

  atlas::symm (cb, ucs, cc); 
  print_m (cc, "c = b a");
  cout << endl; 

  atlas::symm (cb, lcs, cc);  
  print_m (cc, "c = b a");
  cout << endl; 

  atlas::symm (rb, urs, rc); 
  print_m (rc, "c = b a");
  cout << endl; 

  atlas::symm (rb, lrs, rc); 
  print_m (rc, "c = b a");
  cout << endl; 

#ifdef F_COMPILATION_FAILURE

  atlas::symm (ucs, ucs, cc); 
  print_m (cc, "c = a b");
  cout << endl; 

  atlas::symm (urs, cb, cc); 
  print_m (cc, "c = a b");
  cout << endl; 

#endif 

  cout << endl; 

}
