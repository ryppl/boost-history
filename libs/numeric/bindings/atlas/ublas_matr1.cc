
// BLAS level 1 (matrix rows and columns) 

#include <iostream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/atlas/cblas1.hpp>

#include "utils.h"

namespace atlas = boost::numeric::bindings::atlas;
namespace ublas = boost::numeric::ublas;

using std::cout;
using std::endl; 
using std::size_t; 

typedef ublas::vector<double> vct_t;
typedef ublas::matrix<double> matr_t;
typedef ublas::matrix_row<matr_t> mr_t;
typedef ublas::matrix_column<matr_t> mc_t;
typedef ublas::matrix_range<matr_t> mrng_t;
typedef ublas::matrix_slice<matr_t> msl_t;
typedef ublas::matrix_slice<matr_t const> cmsl_t;

int main() {

  cout << endl; 

  int r = 7; 
  int c = 8; 
  matr_t m (r, c);
  init_m (m, 1, 1); 
  print_m (m, "m"); 
  vct_t v (10);
  atlas::set (0., v); 
  print_v (v, "v"); 
  cout << endl; 

  mr_t mr2 (m, 2); 
  atlas::scal (10.0, mr2);
  print_m (m, "m"); 
  
  mr_t mr4 (m, 4);
  atlas::swap (mr2, mr4);
  print_m (m, "m"); 

  atlas::copy (mr2, mr4);
  print_m (m, "m"); 

  mr_t mr5 (m, 5); 
  ublas::vector_range<vct_t> vr (v, ublas::range (2, 6)); 
  ublas::vector_range<mr_t> mr5r (mr5, ublas::range (1, 5)); 
  atlas::axpy (10.0, mr5r, vr);

  print_v (v, "v"); 
  cout << endl; 

  mc_t mc3 (m, 3); 
  cout << atlas::asum (mc3) << " " << atlas::nrm2 (mc3) << endl; 
  
  mc_t mc5 (m, 5); 
  atlas::axpy (0.01, mc3, mc5); 
  print_m (m, "m"); 

  ublas::vector_slice<mc_t> mc5s (mc5, ublas::slice (1, 2, 3)); 
  atlas::scal (0.1, mc5s); 
  print_m (m, "m"); 

  ublas::vector_slice<mr_t> mr5s (mr5, ublas::slice (1, 2, 4));
  ublas::vector_range<ublas::vector_slice<mr_t> >
    mr5sr (mr5s, ublas::range (1, 3)); 
  atlas::scal (0.1, mr5sr); 
  print_m (m, "m"); 
  
  cout << endl; 

  init_m (m, 0.1, 1); 
  matr_t const cm (m); 
  print_m (cm, "cm"); 

  mrng_t mrng (m, ublas::range (2, 6), ublas::range (1, 8)); 
  print_m (mrng, "mrng"); 
  
  ublas::matrix_row<mrng_t> mrngr1 (mrng, 1); 
  ublas::matrix_row<mrng_t> mrngr2 (mrng, 2); 
  
  atlas::swap (mrngr1, mrngr2); 
  print_m (m, "m"); 
  atlas::swap (mrngr2, mrngr1); 

  ublas::matrix_column<mrng_t> mrngc3 (mrng, 3);
  ublas::matrix_column<mrng_t> const mrngc5 (mrng, 5);

  atlas::axpy (0.01, mrngc5, mrngc3); 
  print_m (m, "m"); 

  cmsl_t msl (cm, ublas::slice (1, 2, 3), ublas::slice (2, 3, 2)); 
  print_m (msl, "msl"); 
 
  init_m (m, 0.1, 1); 

  ublas::matrix_column<cmsl_t> mslc0 (msl, 0); 
  
  cout << atlas::asum (mslc0) << endl << endl; 
  
  ublas::vector_range<ublas::matrix_column<mrng_t> >
    vrmrngc3 (mrngc3, ublas::range (1, 4));
  
  atlas::axpy (0.01, mslc0, vrmrngc3); 
  print_m (m, "m"); 
  
  cout << endl; 
}
