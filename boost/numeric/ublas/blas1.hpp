#ifndef boost_ublas_blas1_hpp
#define boost_ublas_blas1_hpp

#include <math/blaspp.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/type_traits.hpp>


namespace boost { namespace numeric { namespace ublas {

  template < typename vector_type >
  BOOST_UBLAS_INLINE
  int stride( const vector_type &v ) {
    return 1 ;
  }
  template < typename vector_type >
  BOOST_UBLAS_INLINE
  int stride( const vector_slice<vector_type> &v ) {
    return v.stride() * stride( v.data() ) ;
  }

  template < typename matrix_type >
  BOOST_UBLAS_INLINE
  int stride1( const matrix_type &m ) {
    return matrix_type::functor_type::one1( m.size1(), m.size2() ) ;
  }
  template < typename matrix_type >
  BOOST_UBLAS_INLINE
  int stride2( const matrix_type &m ) {
    return matrix_type::functor_type::one2( m.size1(), m.size2() ) ;
  }

  template < typename matrix_type >
  BOOST_UBLAS_INLINE
  int stride( const matrix_row<matrix_type> &v ) {
    return stride2( v.data() ) ;
  }
  template < typename matrix_type >
  BOOST_UBLAS_INLINE
  int stride( const matrix_column<matrix_type> &v ) {
    return stride1( v.data() ) ;
  }


  template < typename matrix_vector_type >
  BOOST_UBLAS_INLINE
  typename matrix_vector_type::pointer array_ptr( matrix_vector_type &mv ) {
    return &mv.data().begin()[0] ;
  }
  template < typename matrix_vector_type >
  BOOST_UBLAS_INLINE
  typename matrix_vector_type::const_pointer array_ptr( const matrix_vector_type &mv ) {
    return &mv.data().begin()[0] ;
  }

  template < typename vector_type >
  BOOST_UBLAS_INLINE
  typename vector_type::const_pointer array_ptr( const vector_range<vector_type> &v ) {
    return array_ptr( v.data() ) + v.start() ;
  }
  template < typename vector_type >
  BOOST_UBLAS_INLINE
  typename vector_type::const_pointer array_ptr( const vector_slice<vector_type> &v ) {
    return array_ptr( v.data() ) + v.start() ;
  }
  template < typename vector_type >
  BOOST_UBLAS_INLINE
  typename vector_type::pointer array_ptr( vector_range<vector_type> &v ) {
    return array_ptr( v.data() ) + v.start() ;
  }
  template < typename vector_type >
  BOOST_UBLAS_INLINE
  typename vector_type::pointer array_ptr( vector_slice<vector_type> &v ) {
    return array_ptr( v.data() ) + v.start() ;
  }

  template < typename matrix_type >
  BOOST_UBLAS_INLINE
  typename matrix_type::const_pointer array_ptr( const matrix_row<matrix_type> &v ) {
    return array_ptr( v.data() ) + v.index() * stride1( v.data() ) ;
  }
  template < typename matrix_type >
  BOOST_UBLAS_INLINE
  typename matrix_type::const_pointer array_ptr( const matrix_column<matrix_type> &v ) {
    return array_ptr( v.data() ) + v.index() * stride2( v.data() ) ;
  }
  template < typename matrix_type >
  BOOST_UBLAS_INLINE
  typename matrix_type::pointer array_ptr( matrix_row<matrix_type> &v ) {
    return array_ptr( v.data() ) + v.index() * stride1( v.data() ) ;
  }
  template < typename matrix_type >
  BOOST_UBLAS_INLINE
  typename matrix_type::pointer array_ptr( matrix_column<matrix_type> &v ) {
    return array_ptr( v.data() ) + v.index() * stride2( v.data() ) ;
  }



  template < typename vector_type >
  void scal(const typename vector_type::value_type &alpha, vector_type &x)
  {
    typedef typename vector_type::value_type    value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;
    const int n = x.size() ;
    const int x_stride = stride( x ) ;
    value_type* x_ptr = array_ptr( x ) ;
    blas< value_type >::scal( &n, (bind_type*)&alpha, (bind_type*)x_ptr, &x_stride ) ;
  }

  template < typename vector_type_x, typename vector_type_y >
  void axpy(const typename vector_type_x::value_type alpha, const vector_type_x &x, vector_type_y &y)
  {
    typedef typename vector_type_x::value_type  value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;
    assert( x.size() == y.size() );
    const int n( x.size() );
    const int stride_x = stride( x ) ;
    const int stride_y = stride( y ) ;
    const value_type* x_ptr = array_ptr( x ) ;
    value_type* y_ptr = array_ptr( y ) ;
    blas< value_type >::axpy( &n, (const bind_type*)&alpha, (const bind_type*)x_ptr, &stride_x, (bind_type*)y_ptr, &stride_y );
  }

  template < typename vector_type_x, typename vector_type_y >
  typename vector_type_x::value_type dot(const vector_type_x &x, const vector_type_y &y)
  {
    typedef typename vector_type_x::value_type  value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;
    assert( x.size() == y.size() );
    const int n = x.size();
    const int stride_x = stride( x ) ;
    const int stride_y = stride( y ) ;
    const value_type *x_ptr = array_ptr( x ) ;
    const value_type *y_ptr = array_ptr( y ) ;
    return blas< value_type >::dot( &n, (const bind_type*)x_ptr, &stride_x, (const bind_type*)y_ptr, &stride_y );
  }

  template < typename vector_type_x, typename vector_type_y >
  typename vector_type_x::value_type dotu(const vector_type_x &x, const vector_type_y &y)
  {
    typedef typename vector_type_x::value_type  value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;
    value_type ret = value_type() ;
    assert( x.size() == y.size() );
    const int n = x.size();
    const int stride_x = stride( x ) ;
    const int stride_y = stride( y ) ;
    const value_type *x_ptr = array_ptr( x ) ;
    const value_type *y_ptr = array_ptr( y ) ;
    blas< value_type >::dotu( (bind_type*)&ret, &n, (const bind_type*)x_ptr, &stride_x, (const bind_type*)y_ptr, &stride_y );
    return ret ;
  }

  template < typename vector_type_x, typename vector_type_y >
  typename vector_type_x::value_type dotc(const vector_type_x &x, const vector_type_y &y)
  {
    typedef typename vector_type_x::value_type  value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;
    value_type ret = value_type() ;
    assert( x.size() == y.size() );
    const int n = x.size();
    const int stride_x = x.stride() ;
    const int stride_y = y.stride() ;
    const value_type *x_ptr = x.array_ptr() ;
    const value_type *y_ptr = y.array_ptr() ;
    blas< value_type >::dotc( &ret, &n, (const bind_type*)x_ptr, &stride_x, (const bind_type*)y_ptr, &stride_y );
    return ret ;
  }


}}}

#endif // boost_ublas_blas1_hpp
