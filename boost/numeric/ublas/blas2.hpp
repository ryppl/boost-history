#ifndef boost_ublas_blas2_hpp
#define boost_ublas_blas2_hpp

#include <math/blaspp.hpp>
#include <math/blas1.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/type_traits.hpp>


namespace boost { namespace numeric { namespace ublas {

  static const char NO_TRANSPOSE = 'N' ;
  static const char TRANSPOSE    = 'T' ;
  static const char CONJUGATE    = 'C' ;

  template < typename matrix_type, typename vector_type_x, typename vector_type_y >
  void gemv(const char TRANS, const typename matrix_type::value_type &alpha, const matrix_type &a, const vector_type_x &x, const typename vector_type_y::value_type &beta, vector_type_y &y)
  {
    typedef typename matrix_type::value_type                value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;
    int m = y.size() ;
    int n = x.size() ;
    int lda = m ;
    int stride_x = stride( x ) ;
    int stride_y = stride( y ) ;
    const value_type *a_ptr = array_ptr( a ) ;
    const value_type *x_ptr = array_ptr( x ) ;
    value_type *y_ptr = array_ptr( y ) ;
    blas< value_type >::gemv(&TRANS,&m,&n,(const bind_type*)&alpha,(const bind_type*)a_ptr,&lda,(const bind_type*)x_ptr,&stride_x,(const bind_type*)&beta,(bind_type*)y_ptr,&stride_y);
  }

}}}

#endif // boost_ublas_blas2_hpp
