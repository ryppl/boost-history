
// BLAS level 1 (vectors) 

#include <iostream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/atlas/cblas1.hpp>
#ifdef USE_STD_VECTOR
#include <vector>
#include <boost/numeric/bindings/traits/std_vector.hpp> 
#endif 

#include "utils.h"

namespace atlas = boost::numeric::bindings::atlas;
namespace ublas = boost::numeric::ublas;

using std::cout;
using std::endl; 
using std::size_t; 

#ifndef USE_STD_VECTOR
typedef ublas::vector<double> vct_t;
#else
typedef ublas::vector<double, std::vector<double> > vct_t;
#endif 

int main() {

  cout << endl; 

  vct_t v (10);
  init_v (v, 0.1); 
  print_v (v, "v"); 
  vct_t v1 (10);
  init_v (v1, 10.1); 
  print_v (v1, "v1"); 
  vct_t vy (10); 
  atlas::set (0., vy); 
  print_v (vy, "vy"); 
  cout << endl; 

  atlas::scal (2.0, v); 
  print_v (v, "2 v");

  atlas::axpy (2.0, v1, vy); 
  print_v (vy, "vy = 2 v1"); 

  cout << "v v1: " << atlas::dot (v, v1) << endl;
  cout << endl; 

  /////////////////
  ublas::vector_range<vct_t> vr (v, ublas::range (2, 6)); 
  ublas::vector_range<vct_t> vry (vy, ublas::range (4, 8)); 


  init_v (v, 0.1); 
  atlas::scal (10.0, vr); 
  print_v (v, "10 vr"); 

  cout << "vr vr: " << atlas::dot (vr, vr) << endl;

  atlas::set (0., vy); 
  atlas::axpy (0.01, vr, vry); 

  print_v (vy, "vry = 0.01 vr"); 
  cout << endl; 

  /////////////////
  ublas::vector_slice<vct_t> vs (v, ublas::slice (1, 2, 4)); 
  ublas::vector_slice<vct_t> vsy (vy, ublas::slice (2, 2, 4)); 

  init_v (v, 0.1); 
  atlas::scal (10.0, vs); 
  print_v (v, "10 vs"); 

  cout << "vs vs: " << atlas::dot (vs, vs) << endl;

  atlas::set (0., vy); 
  atlas::axpy (0.01, vs, vsy); 
  print_v (vy, "vsy = 0.01 vs"); 
  cout << endl; 

  atlas::set (0., vy); 
  atlas::axpy (0.01, vr, vsy); 
  print_v (vy, "vsy = 0.01 vr"); 

  atlas::set (0., vy); 
  atlas::axpy (0.01, vs, vry); 
  print_v (vy, "vry = 0.01 vs"); 

  cout << "vr vs: " << atlas::dot (vr, vs) << " == " 
       << atlas::dot (vs, vr) << endl;

  cout << endl; 

  ///////////////
  init_v (v, 0.1); 
  ublas::vector_range<vct_t> vr1 (v, ublas::range (1, 9)); 
  ublas::vector_slice< 
    ublas::vector_range<vct_t> 
  > vsr (vr1, ublas::slice (1, 2, 3)); 
  print_v (vsr, "vs(vr)"); 

  atlas::scal (0.1, vsr); 
  print_v (v, "0.1 vs(vr)"); 

  cout << "||vs (vr)||: " << atlas::nrm2 (vsr) << endl; 

  cout << endl; 

  atlas::set (0., vy); 
  ublas::vector_slice<vct_t> vsy1 (vy, ublas::slice (0, 2, 5));
  ublas::vector_range<
    ublas::vector_slice<vct_t> 
  > vrs (vsy1, ublas::range (1, 4)); 

  atlas::axpy (0.01, vsr, vrs); 
  print_v (vy, "vr(vs) = 0.01 vs(vr)"); 

  cout << endl; 

}
