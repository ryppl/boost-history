
// BLAS level 1 (vector) -- complex numbers

#include <iostream>
#include <cmath>
#include <complex> 

#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/atlas/cblas1.hpp>
#include "utils.h" 

namespace atlas = boost::numeric::bindings::atlas;
namespace ublas = boost::numeric::ublas;

using std::cout;
using std::endl; 

typedef double real_t;
typedef std::complex<real_t> cmplx_t;  
typedef ublas::vector<cmplx_t> vct_t; 

int main() {

  int n = 6; 

  cout << endl; 
  vct_t v (n); 
  init_v (v, times_plus<cmplx_t> (cmplx_t (1, -1), cmplx_t (0, .1))); 
  print_v (v, "v"); 

  atlas::scal (2.0, v); 
  print_v (v, "2.0 v"); 

  atlas::scal (cmplx_t (-1, 0), v); 
  print_v (v, "(-1, 0) v"); 

  atlas::scal (cmplx_t (0, 1), v);
  print_v (v, "(0, 1) v"); 

  atlas::set (cmplx_t (1, -1), v); 
  print_v (v, "v"); 

  cout << "v v = " << atlas::dot (v, v) << endl; 
  cout << "||v||_1 = " << atlas::asum (v) << endl; 
  cout << "||v||_2 = " << atlas::nrm2 (v) << endl; 
  
  cout << endl;
}
