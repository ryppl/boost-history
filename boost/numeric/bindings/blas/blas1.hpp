//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright Toon Knapen and Kresimir Fresl

#ifndef BOOST_NUMERIC_BINDINGS_BLAS_BLAS1_HPP
#define BOOST_NUMERIC_BINDINGS_BLAS_BLAS1_HPP

#include <cassert> 

#include <boost/numeric/bindings/blas/blas1_overloads.hpp>

namespace boost { namespace numeric { namespace bindings { namespace blas {

  // x <- alpha * x
  template < typename vector_type >
  void scal(const typename traits::vector_traits<vector_type>::value_type &alpha, 
	    vector_type &x
	    )
  {
    typedef traits::vector_traits< vector_type > vtraits ; 
    typedef typename vtraits::value_type value_type ;
    typedef typename traits::value_traits< value_type >::value_type bind_type ;

    const int n =  vtraits::size( x ) ;
    const int stride = vtraits::stride( x ) ;
    value_type *x_ptr = vtraits::storage( x ) ;

    detail::scal( n, alpha, x_ptr, stride ) ;
  }


  // y <- alpha * x + y
  template < typename vector_type_x, typename vector_type_y >
  void axpy(const typename traits::vector_traits<vector_type_x>::value_type alpha,
	    const vector_type_x &x, 
	    vector_type_y &y
	    )
  { 
    typedef traits::vector_traits< const vector_type_x > xvtraits ; 
    typedef traits::vector_traits< vector_type_y > yvtraits ; 
    typedef typename xvtraits::value_type  value_type ;
    typedef typename traits::value_traits< value_type >::value_type bind_type ;

    const int n = xvtraits::size( x ) ;
    assert( n == yvtraits::size( y ) ) ;
    const int stride_x = xvtraits::stride( x ) ;
    const int stride_y = yvtraits::stride( y ) ;
    const value_type *x_ptr = xvtraits::storage( x ) ;
    value_type *y_ptr = yvtraits::storage( y ) ;

    detail::axpy( n, alpha, x_ptr, stride_x, y_ptr, stride_y ) ; 
  }


  // dot <- x^T * y  (real vectors)
  template < typename vector_type_x, typename vector_type_y >
  typename traits::vector_traits< vector_type_x >::value_type 
  dot(const vector_type_x &x, const vector_type_y &y)
  {
    typedef traits::vector_traits< const vector_type_x > xvtraits ; 
    typedef traits::vector_traits< const vector_type_y > yvtraits ; 
    typedef typename xvtraits::value_type  value_type ;
    typedef typename traits::value_traits< value_type >::value_type bind_type ;

    const int n = xvtraits::size( x ) ;
    assert( n == yvtraits::size( y ) ) ;
    const int stride_x = xvtraits::stride( x ) ;
    const int stride_y = yvtraits::stride( y ) ;
    const value_type *x_ptr = xvtraits::storage( x ) ;
    const value_type *y_ptr = yvtraits::storage( y ) ;

    return blas< value_type >::dot( &n, 
				    (const bind_type*)x_ptr, &stride_x, 
				    (const bind_type*)y_ptr, &stride_y 
				    ) ;
  }

  // dotu <- x^T * y  (complex vectors)
  template < typename vector_type_x, typename vector_type_y >
  typename traits::vector_traits< vector_type_x >::value_type 
  dotu(const vector_type_x &x, const vector_type_y &y)
  {
    typedef traits::vector_traits< const vector_type_x > xvtraits ; 
    typedef traits::vector_traits< const vector_type_y > yvtraits ; 
    typedef typename xvtraits::value_type  value_type ;
    typedef typename traits::value_traits< value_type >::value_type bind_type ;

    const int n = xvtraits::size( x ) ;
    assert( n == yvtraits::size( y ) ) ;
    const int stride_x = xvtraits::stride( x ) ;
    const int stride_y = yvtraits::stride( y ) ;
    const value_type *x_ptr = xvtraits::storage( x ) ;
    const value_type *y_ptr = yvtraits::storage( y ) ;
    
    value_type ret ;
    blas< value_type >::dotu( (bind_type*)&ret,
                              &n, 
                              (const bind_type*)x_ptr, &stride_x, 
                              (const bind_type*)y_ptr, &stride_y 
                              ) ;
    return ret;
  }

  // dotc <- x^H * y  (complex vectors) 
  template < typename vector_type_x, typename vector_type_y >
  typename traits::vector_traits< vector_type_x >::value_type 
  dotc(const vector_type_x &x, const vector_type_y &y)
  {
    typedef traits::vector_traits< const vector_type_x > xvtraits ; 
    typedef traits::vector_traits< const vector_type_y > yvtraits ; 
    typedef typename xvtraits::value_type  value_type ;
    typedef typename traits::value_traits< value_type >::value_type bind_type ;

    const int n = xvtraits::size( x ) ;
    assert( n == yvtraits::size( y ) ) ;
    const int stride_x = xvtraits::stride( x ) ;
    const int stride_y = yvtraits::stride( y ) ;
    const value_type *x_ptr = xvtraits::storage( x ) ;
    const value_type *y_ptr = yvtraits::storage( y ) ;

    value_type ret ;
    blas< value_type >::dotc( (bind_type*)&ret,
                              &n, 
                              (const bind_type*)x_ptr, &stride_x, 
                              (const bind_type*)y_ptr, &stride_y 
                              ) ;
    return ret ;
  }


  // nrm2 <- ||x||_2
  // .. for now works only with real vectors
  template < typename vector_type >
  typename traits::vector_traits< vector_type >::value_type 
  nrm2(const vector_type &x) 
  {
    typedef traits::vector_traits< const vector_type > vtraits ; 
    typedef typename vtraits::value_type  value_type ;
    typedef typename traits::value_traits< value_type >::value_type bind_type ;

    const int n = vtraits::size( x ) ;
    const int stride_x = vtraits::stride( x ) ;
    const value_type *x_ptr = vtraits::storage( x ) ;

    return 
      blas< value_type >::nrm2( &n, (const bind_type*)x_ptr, &stride_x ) ;
  }


  // asum <- ||x||_1
  // .. for now works only with real vectors
  template < typename vector_type >
  typename traits::vector_traits< vector_type >::value_type 
  asum(const vector_type &x) 
  {
    typedef traits::vector_traits< const vector_type > vtraits ; 
    typedef typename vtraits::value_type  value_type ;
    typedef typename traits::value_traits< value_type >::value_type bind_type ;

    const int n = vtraits::size( x ) ;
    const int stride_x = vtraits::stride( x ) ;
    const value_type *x_ptr = vtraits::storage( x ) ;

    return 
      blas< value_type >::asum( &n, (const bind_type*)x_ptr, &stride_x ) ;
  }

}}}}

#endif // BOOST_NUMERIC_BINDINGS_BLAS_BLAS1_HPP
