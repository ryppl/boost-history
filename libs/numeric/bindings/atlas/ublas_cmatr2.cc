
// BLAS level 2 -- complex numbers

//#define USE_STD_VECTOR

#include <iostream>
#include <complex>
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

typedef double real_t;
typedef std::complex<real_t> cmplx_t; 

#ifndef USE_STD_VECTOR
typedef ublas::vector<cmplx_t> vct_t;
typedef ublas::matrix<cmplx_t, ublas::row_major> m_t;
#else
typedef ublas::vector<cmplx_t, std::vector<cmplx_t> > vct_t;
typedef ublas::matrix<cmplx_t, ublas::column_major, std::vector<cmplx_t> > m_t;
#endif 

int main() {

  cout << endl; 

  vct_t v (2);
  atlas::set (cmplx_t (1., 0.), v);
  cout << v << endl; 
  vct_t vy (4); 
  atlas::set (cmplx_t (0., 0.), vy); 
  cout << vy << endl; 
  cout << endl; 

  m_t m (3, 2);
  m (0, 0) = cmplx_t (1., 0.);
  m (0, 1) = cmplx_t (2., 0.);
  m (1, 0) = cmplx_t (3., 0.);
  m (1, 1) = cmplx_t (4., 0.);
  m (2, 0) = cmplx_t (5., 0.);
  m (2, 1) = cmplx_t (6., 0.);
  cout << m << endl;

  atlas::gemv (CblasNoTrans, 1.0, m, v, 0.0, vy);
  cout << vy << endl; 
  atlas::gemv (m, v, vy);
  cout << vy << endl; 

  cout << endl; 

  m (0, 0) = cmplx_t (0., 1.);
  m (0, 1) = cmplx_t (0., 2.);
  m (1, 0) = cmplx_t (0., 3.);
  m (1, 1) = cmplx_t (0., 4.);
  m (2, 0) = cmplx_t (0., 5.);
  m (2, 1) = cmplx_t (0., 6.);
  cout << m << endl;

  atlas::gemv (CblasNoTrans, 1.0, m, v, 0.0, vy);
  cout << vy << endl; 
  atlas::gemv (m, v, vy);
  cout << vy << endl; 

  cout << endl; 

  m (0, 0) = cmplx_t (-1., 1.);
  m (0, 1) = cmplx_t (-2., 2.);
  m (1, 0) = cmplx_t (-3., 3.);
  m (1, 1) = cmplx_t (-4., 4.);
  m (2, 0) = cmplx_t (-5., 5.);
  m (2, 1) = cmplx_t (-6., 6.);
  cout << m << endl;

  atlas::gemv (CblasNoTrans, 1.0, m, v, 0.0, vy);
  cout << vy << endl; 
  atlas::gemv (m, v, vy);
  cout << vy << endl; 

  cout << endl; 

  atlas::set (cmplx_t (0., 1.), v);
  cout << v << endl; 

  m (0, 0) = cmplx_t (-1., 1.);
  m (0, 1) = cmplx_t (-2., 2.);
  m (1, 0) = cmplx_t (-3., 3.);
  m (1, 1) = cmplx_t (-4., 4.);
  m (2, 0) = cmplx_t (-5., 5.);
  m (2, 1) = cmplx_t (-6., 6.);
  cout << m << endl;

  atlas::gemv (CblasNoTrans, 1.0, m, v, 0.0, vy);
  cout << vy << endl; 
  atlas::gemv (m, v, vy);
  cout << vy << endl; 

  cout << endl; 

  atlas::set (cmplx_t (1., 1.), v);
  cout << v << endl; 

  m (0, 0) = cmplx_t (-1., 1.);
  m (0, 1) = cmplx_t (-2., 2.);
  m (1, 0) = cmplx_t (-3., 3.);
  m (1, 1) = cmplx_t (-4., 4.);
  m (2, 0) = cmplx_t (-5., 5.);
  m (2, 1) = cmplx_t (-6., 6.);
  cout << m << endl;

  atlas::gemv (CblasNoTrans, 1.0, m, v, 0.0, vy);
  cout << vy << endl; 
  atlas::gemv (m, v, vy);
  cout << vy << endl; 

  cout << endl; 

  m_t mx (2, 2); 
  m_t my (3, 2); 

  ublas::matrix_column<m_t> mxc0 (mx, 0), mxc1 (mx, 1); 
  ublas::matrix_column<m_t> myc0 (my, 0), myc1 (my, 1); 

  atlas::set (cmplx_t (1., 0.), mxc0);
  atlas::set (cmplx_t (0., 0.), mxc1);
  atlas::set (cmplx_t (0., 0.), myc0);
  atlas::set (cmplx_t (0., 0.), myc1);

  cout << mx << endl;
  cout << my << endl;

  atlas::gemv (m, mxc0, myc0); 
  cout << my << endl;

  cout << endl;

}
