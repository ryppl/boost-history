#ifndef boost_ublas_blasbindings_hpp
#define boost_ublas_blasbindings_hpp

#include <boost/numeric/ublas/blaspp.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/type_traits.hpp>


namespace boost { namespace numeric { namespace ublas {
  
  const char NO_TRANSPOSE = 'N' ;
  const char TRANSPOSE    = 'T' ;
  const char CONJUGATE    = 'C' ;
  


  template < typename vector_type >
  void scal(const typename vector_type::value_type &alpha, vector_type &x)
  {
    typedef typename vector_type::value_type    value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;
    const int n = x.size() ;
    const int stride = x.stride() ;
    value_type* x_ptr = x.array_ptr() ;
    blas< value_type >::scal( &n, (bind_type*)&alpha, (bind_type*)x_ptr, &stride ) ;
  }

  template < typename vector_type_x, typename vector_type_y >
  void axpy(const typename vector_type_x::value_type alpha, const vector_type_x &x, vector_type_y &y)
  { 
    typedef typename vector_type_x::value_type  value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;
    assert( x.size() == y.size() );
    const int n( x.size() );
    const int stride_x( x.stride() );
    const int stride_y( y.stride() );
    const value_type* x_ptr = x.array_ptr() ;
    value_type* y_ptr = y.array_ptr() ;
    blas< value_type >::axpy( &n, (const bind_type*)&alpha, (const bind_type*)x_ptr, &stride_x, (bind_type*)y_ptr, &stride_y );
  }

  template < typename vector_type_x, typename vector_type_y >
  typename vector_type_x::value_type dot(const vector_type_x &x, const vector_type_y &y)
  {
    typedef typename vector_type_x::value_type  value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;
    assert( x.size() == y.size() );
    const int n = x.size();
    const int stride_x = x.stride() ;
    const int stride_y = y.stride() ;
    const value_type *x_ptr = x.array_ptr() ;
    const value_type *y_ptr = y.array_ptr() ;
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
    const int stride_x = x.stride() ;
    const int stride_y = y.stride() ;
    const value_type *x_ptr = x.array_ptr() ;
    const value_type *y_ptr = y.array_ptr() ;
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

  template < typename matrix_type, typename vector_type_x, typename vector_type_y >
  void gemv(const typename matrix_type::value_type &alpha, const matrix_type &a, const vector_type_x &x, const typename vector_type_y::value_type &beta, vector_type_y &y)
  {
    typedef typename matrix_type::value_type    value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;
    assert( a.size2() == x.size() );
    assert( x.size() == y.size() );
    int m = a.size1() ;
    int n = a.size2() ;
    int lda = m ;
    int stride_x = x.stride() ;
    int stride_y = y.stride() ;
    const value_type *a_ptr = a.array_ptr() ;
    blas< value_type >::gemv(&NO_TRANSPOSE,&m,&n,(const bind_type*)&alpha,(const bind_type*)a_ptr,&lda,(const bind_type*)&*(x.array_ptr()),&stride_x,(const bind_type*)&beta,(bind_type*)&*(y.array_ptr()),&stride_y);
  }

  template < typename matrix_type, typename vector_type_x, typename vector_type_y >
  void gemv(const typename matrix_type::value_type &alpha, const matrix_unary2< const matrix_const_reference< matrix_type >, scalar_identity< typename matrix_type::value_type > > &a, const vector_type_x &x, const typename vector_type_y::value_type &beta, vector_type_y &y)
  {
    typedef typename matrix_type::value_type    value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;
    assert( a.size2() == x.size() );
    assert( x.size() == y.size() );
    int m = a.expression().size1() ;
    int n = a.expression().size2() ;
    int lda = m ;
    int stride_x = x.stride() ;
    int stride_y = y.stride() ;
    const value_type *a_ptr = a.expression().expression().array_ptr() ;
    blas< value_type >::gemv(&TRANSPOSE,&m,&n,(const bind_type*)&alpha,(const bind_type*)a_ptr,&lda,(const bind_type*)&*(x.array_ptr()),&stride_x,(const bind_type*)&beta,(bind_type*)&*(y.array_ptr()),&stride_y);
  }

  template < typename matrix_type_a, typename matrix_type_b, typename matrix_type_c >
  void gemm(const typename matrix_type_a::value_type& alpha, const matrix_type_a &a, const matrix_type_b &b, const typename matrix_type_c::value_type &beta, matrix_type_c &c)
  {
    typedef typename matrix_type_c::value_type  value_type ;
    typedef typename value_traits< value_type >::value_type bind_type ;

    assert( a.size1() == c.size1() ); // m
    assert( b.size2() == c.size2() ); // n
    assert( a.size2() == b.size1() ); // k
    const int m = a.size1();
    const int n = b.size2();
    const int k = a.size2();
    const int lda = m;
    const int ldb = k;
    const int ldc = m;
    const value_type *a_ptr = a.array_ptr() ;
    const value_type *b_ptr = b.array_ptr() ;
    value_type *c_ptr       = c.array_ptr() ;
    blas< value_type >::gemm( &NO_TRANSPOSE, &NO_TRANSPOSE, &m, &n, &k, (const bind_type*)&alpha, (const bind_type*)a_ptr, &lda, (const bind_type*)b_ptr, &ldb, (const bind_type*)&beta, (bind_type*)c_ptr, &ldc );
  }

}}}

#endif // boost_ublas_blasbindings_hpp
