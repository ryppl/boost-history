#ifndef BOOST_BLASBINDINGS_BLAS1_HPP
#define BOOST_BLASBINDINGS_BLAS1_HPP

#include <cassert> 

#include <boost/numeric/blasbindings/blaspp.hpp>
#include <boost/numeric/blasbindings/traits.hpp>

namespace boost { namespace numeric { namespace blasbindings {
  
  template < typename vector_type >
  void scal(const typename vector_traits<vector_type>::value_type &alpha, 
	    vector_type &x
	    )
  {
    typedef vector_traits< vector_type >                           vtraits ; 
    typedef typename vtraits::value_type                    value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

    const int n =  vtraits::size( x ) ;
    const int stride = vtraits::stride( x ) ;
    value_type *x_ptr = vtraits::storage( x ) ;

    blas< value_type >::scal( &n, 
			      (bind_type*)&alpha, 
			      (bind_type*)x_ptr, &stride 
			      ) ;
  }


  template < typename vector_type_x, typename vector_type_y >
  void axpy(const typename vector_traits<vector_type_x>::value_type alpha,
	    const vector_type_x &x, 
	    vector_type_y &y
	    )
  { 
    typedef vector_traits< const vector_type_x >                   xvtraits ; 
    typedef vector_traits< vector_type_y >                         yvtraits ; 
    typedef typename xvtraits::value_type                   value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

    const int n = xvtraits::size( x ) ;
    assert( n == static_cast< int >( yvtraits::size( y ) ) ) ;
    const int stride_x = xvtraits::stride( x ) ;
    const int stride_y = yvtraits::stride( y ) ;
    const value_type *x_ptr = xvtraits::storage( x ) ;
    value_type *y_ptr = yvtraits::storage( y ) ;

    blas< value_type >::axpy( &n, 
			      (const bind_type*)&alpha, 
			      (const bind_type*)x_ptr, &stride_x, 
			      (bind_type*)y_ptr, &stride_y 
			      ) ;
  }


  template < typename vector_type_x, typename vector_type_y >
  typename vector_traits< vector_type_x >::value_type 
  dot(const vector_type_x &x, const vector_type_y &y)
  {
    typedef vector_traits< const vector_type_x >                   xvtraits ; 
    typedef vector_traits< const vector_type_y >                   yvtraits ; 
    typedef typename xvtraits::value_type                   value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

    const int n = xvtraits::size( x ) ;
    assert( n == static_cast< int >( yvtraits::size( y ) ) ) ;
    const int stride_x = xvtraits::stride( x ) ;
    const int stride_y = yvtraits::stride( y ) ;
    const value_type *x_ptr = xvtraits::storage( x ) ;
    const value_type *y_ptr = yvtraits::storage( y ) ;

    return blas< value_type >::dot( &n, 
				    (const bind_type*)x_ptr, &stride_x, 
				    (const bind_type*)y_ptr, &stride_y 
				    ) ;
  }

  template < typename vector_type_x, typename vector_type_y >
  typename vector_traits< vector_type_x >::value_type 
  dotu(const vector_type_x &x, const vector_type_y &y)
  {
    typedef vector_traits< const vector_type_x >                   xvtraits ; 
    typedef vector_traits< const vector_type_y >                   yvtraits ; 
    typedef typename xvtraits::value_type                   value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

    const int n = xvtraits::size( x ) ;
    assert( n == static_cast< int >( yvtraits::size( y ) ) ) ;
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

  template < typename vector_type_x, typename vector_type_y >
  typename vector_traits< vector_type_x >::value_type 
  dotc(const vector_type_x &x, const vector_type_y &y)
  {
    typedef vector_traits< const vector_type_x >                   xvtraits ; 
    typedef vector_traits< const vector_type_y >                   yvtraits ; 
    typedef typename xvtraits::value_type                   value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

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
}}}

#endif // BOOST_BLASBINDINGS_BLAS1_HPP
