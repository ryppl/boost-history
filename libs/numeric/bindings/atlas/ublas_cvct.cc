
// BLAS level 1 (vector) -- complex numbers

#include <iostream>
#include <cmath>
#include <complex> 

#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/atlas/cblas1.hpp>

namespace atlas = boost::numeric::bindings::atlas;
namespace ublas = boost::numeric::ublas;

using std::cout;
using std::endl; 

typedef double real_t;
typedef std::complex<real_t> cmplx_t;  
typedef ublas::vector<cmplx_t> vct_t; 

template <typename V>
void init (V& v, real_t s = 1.) {
  size_t size = v.size();
  for(size_t i = 0 ; i < size ; ++i ) {
    cmplx_t c (s * i, s / (i + 1)); 
    v[i] = c; 
  }
}

template <typename V>
void print_v (V const& va) {
  for (int i = 0; i < va.size(); ++i)
    cout << va[i] << " ";
  cout << endl; 
}

int main() {

  int n = 6; 

  cout << endl; 
  vct_t v (n); 
  init (v, 0.1); 
  print_v (v); 

  atlas::scal (2.0, v); 
  print_v (v); 

  init (v, 0.1); 
  atlas::scal (cmplx_t (-1, 0), v); 
  print_v (v); 

  init (v, 0.1); 
  atlas::scal (cmplx_t (0, 1), v);
  print_v (v); 

  atlas::set (cmplx_t (1, -1), v); 
  print_v (v); 

  cout << atlas::dot (v, v) << endl; 
  
  cout << endl;
}
