
// BLAS level 2

//#define USE_STD_VECTOR

#include <iostream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/bindings/atlas/cblas1.hpp>
#include <boost/numeric/bindings/atlas/cblas2.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#ifdef USE_STD_VECTOR
#include <vector>
#include <boost/numeric/bindings/traits/std_vector.hpp> 
#endif 

namespace ublas = boost::numeric::ublas;
namespace atlas = boost::numeric::bindings::atlas;

using std::cout;
using std::endl; 

#ifndef USE_STD_VECTOR
typedef ublas::vector<double> vct_t;
typedef ublas::matrix<double, ublas::row_major> m_t;
#else
typedef ublas::vector<double, std::vector<double> > vct_t;
typedef ublas::matrix<double, ublas::column_major, std::vector<double> > m_t;
#endif 

int main() {

  cout << endl; 

  vct_t v (2);
  atlas::set (1., v);
  cout << v << endl; 
  vct_t vy (4); 
  atlas::set (0., vy); 
  cout << vy << endl; 
  cout << endl; 

  m_t m (3, 2);
  m (0, 0) = 1.;
  m (0, 1) = 2.;
  m (1, 0) = 3.;
  m (1, 1) = 4.;
  m (2, 0) = 5.;
  m (2, 1) = 6.;
  cout << m << endl;
  cout << endl; 

  atlas::gemv (CblasNoTrans, 1.0, m, v, 0.0, vy);
  cout << vy << endl; 

  atlas::set (0, vy); 
  atlas::gemv (m, v, vy);
  cout << vy << endl; 

  atlas::set (0, vy); 
  atlas::gemv (1.0, m, v, 0.0, vy);
  cout << vy << endl; 
  cout << endl; 

  atlas::set (0, v); 
  atlas::set (1, vy); 
  atlas::gemv (CblasTrans, 1.0, m, vy, 0.0, v);
  cout << v << endl; 
  cout << endl; 

  atlas::set (1, vy); 
  atlas::gemv (CblasNoTrans, 2.0, m, v, 0.5, vy);
  cout << vy << endl;  
  cout << endl; 

  atlas::set (1, v); 
  atlas::set (0, vy); 

  ublas::matrix_range<m_t> mr (m, ublas::range (0, 2), ublas::range (0, 2)); 
  cout << mr << endl;
  cout << endl; 
  
  atlas::gemv (mr, v, vy); 
  cout << vy << endl;  
  cout << endl; 

  atlas::gemv (CblasTrans, 1.0, mr, v, 0.0, vy);
  cout << vy << endl; 
  cout << endl; 

  cout << endl; 
}
