
// BLAS level 1
// std::vector<>, std::valarray<>, boost::array<>, C array

#include <iostream>
#include <iterator>
#include <algorithm>
#include <complex>

#include <boost/numeric/bindings/traits/std_vector.hpp>
#include <boost/numeric/bindings/traits/std_valarray.hpp>
#include <boost/numeric/bindings/traits/boost_array.hpp>
#include <boost/numeric/bindings/traits/c_array.hpp>
#include <boost/numeric/bindings/atlas/cblas1.hpp>

#include "utils.h"

namespace atlas = boost::numeric::bindings::atlas;

using std::cout;
using std::endl;
using std::size_t; 

typedef float real_t; 

int main() {

  int n = 10; 

  cout << endl; 
  cout << "std::vector" << endl; 
  std::vector<real_t> sv (n); 
  init_v (sv, 0.1); 
  print_v (sv, "sv"); 
  cout << "std::valarray" << endl; 
  std::valarray<real_t> va (n); 
  atlas::set (1., va); 
  print_v (va, "va"); 
  cout << endl; 

  real_t d = 0;
  for (int i = 0; i < n; ++i)
    d += sv[i] * va[i]; 

  cout << "Is " << d << " == " << atlas::dot (sv, va) << " ?" << endl; 
  cout << endl; 

  cout << "10 sv^T va: " << atlas::sdsdot (10, sv, va) << endl; 
  cout << endl; 

  atlas::scal (2, sv);
  print_v (sv, "2 sv"); 

  cout << endl; 

  std::random_shuffle (sv.begin(), sv.end());
  cout << "shuffled sv: "; 
  std::copy (sv.begin(), sv.end(), std::ostream_iterator<real_t> (cout, " ")); 
  cout << endl; 
  int i = atlas::iamax (sv); 
  cout << "index of max el: " << i << ", max el: " << sv[i] << endl; 
  cout << endl; 

  cout << "||.||_1: " << atlas::asum (sv) << " " << atlas::asum (va) << endl; 
  cout << "||.||_2: " << atlas::nrm2 (sv) << " " << atlas::nrm2 (va) << endl; 
  cout << endl; 

  boost::array<double, 10> ba;
  double ca[10]; 
  atlas::set (0.1, ba);
  atlas::set (1., ca); 
  cout << "boost::array" << endl;
  print_v (ba, "ba");
  cout << "C array" << endl; 
  print_v (ca, "ca");
  cout << endl; 
  
  atlas::axpy (0.1, ba, ca); 
  print_v (ca, "0.1 ba + ca"); 

  atlas::set (1., ca); 
  atlas::axpby (0.1, ba, 2., ca); 
  print_v (ca, "0.1 ba + 2. ca"); 

  cout << endl;
}
