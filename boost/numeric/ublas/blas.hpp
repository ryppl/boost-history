#ifndef boost_ublas_blas_hpp
#define boost_ublas_blas_hpp

#include <complex>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/matrix.h>

  //
  // make sure to match the library symbols
  // taking the right convention to link C with Fortran
#if defined(__GNUC__) || defined(__ICC)
#define BLAS_SAXPY saxpy_
#define BLAS_DAXPY daxpy_
#define BLAS_CAXPY caxpy_
#define BLAS_ZAXPY zaxpy_

#define BLAS_DDOT ddot_
#define BLAS_ZDOT zdotc_

#define BLAS_DGEMM dgemm_
#define BLAS_ZGEMM zgemm_
#else
#error do not how to link with fortran
#endif
  
  //
  // define C-wrapper for (fortran) BLAS calls
extern "C"
{
  void BLAS_SAXPY(const int *n, const float*                  alpha, const float*                  x, const int* incx,  float*                  y, const int* incy);
  void BLAS_DAXPY(const int *n, const double*                 alpha, const double*                 x, const int* incx,  double*                 y, const int* incy);
  void BLAS_CAXPY(const int *n, const std::complex< float >*  alpha, const std::complex< float >*  x, const int* incx,  std::complex< float >*  y, const int* incy);
  void BLAS_ZAXPY(const int *n, const std::complex< double >* alpha, const std::complex< double >* x, const int* incx,  std::complex< double >* y, const int* incy);

  double                 BLAS_DDOT(const int *n, const double                 *x, const int *incx, const double                 *y, const int *incy);
  std::complex< double > BLAS_ZDOT(const int *n, const std::complex< double > *x, const int *incx, const std::complex< double > *y, const int *incy);

  void BLAS_DGEMM(const char *transa, const char *transb, const int *m, const int *n, const int *k, const double                 *alpha, const double                 *a, const int *lda, const double                 *b, const int *ldb, const double                 *beta, double                 *c, const int *ldc);
  void BLAS_ZGEMM(const char *transa, const char *transb, const int *m, const int *n, const int *k, const std::complex< double > *alpha, const std::complex< double > *a, const int *lda, const std::complex< double > *b, const int *ldb, const std::complex< double > *beta, std::complex< double > *c, const int *ldc);
}

namespace boost { namespace numerics { 
  
  template < typename T >
  struct blas_traits
  {
    // level 1 types
    typedef void (*axpy_type)(const int *n, const T* alpha, const T* x, const int* incx,  T* y, const int* incy);
    typedef T (*dot_type)(const int *n, const T* x, const int* incx,  const T* y, const int* incy);

    // level 2 types
    
    // level 3 types
    typedef void (*gemm_type)(const char *transa, const char *transb, const int *m, const int *n, const int *k, const T *alpha, const T *a, const int *lda, const T *b, const int *ldb, const T *beta, T *c, const int *ldc);

    // level 1 functions
    static axpy_type axpy ;
    static dot_type dot ;

    // level 2 functions

    // level 3 functions
    static gemm_type gemm ;
  };
  
  const int one = 1;
  const char TRANS_N = 'N';
  const char TRANS_T = 'T';
  const char TRANS_C = 'C';
  
  template < typename T >
  void axpy(const T& alpha, const vector< T >& x, vector< T >& y)
  { 
    assert( x.size() == y.size() );
    const int n = x.size();
    blas_traits< T >::axpy( &n, &alpha, &x(0), &one, &y(0), &one );
  }

  template < typename T >
  T dot(const vector< T >& x, const vector< T >& y)
  {
    assert( x.size() == y.size() );
    const int n = x.size();
    return blas_traits< T >::dot( &n, &x(0), &one, &y(0), &one );
  }

  template < typename T >
  void gemm(const T& alpha, const matrix< T, column_major >& a, const matrix< T, column_major >& b, const T& beta, matrix< T, column_major >& c)
  {
    assert( a.size1() == c.size1() ); // m
    assert( b.size2() == c.size2() ); // n
    assert( a.size2() == b.size1() ); // k
    const int m = a.size1();
    const int n = b.size2();
    const int k = a.size2();
    const int lda = m;
    const int ldb = k;
    const int ldc = m;
    blas_traits< T >::gemm( &TRANS_N, &TRANS_N, &m, &n, &k, &alpha, &(a.data()[0]), &lda, &(b.data()[0]), &ldb, &beta, &(c.data()[0]), &ldc );
  }

  template < typename T >
  void gemm(const T& alpha, 
            const matrix< T, column_major >& a, 
            const typename matrix_unary2_traits< matrix< T, column_major >, scalar_identity< T > >::result_type & b, 
            const T& beta, 
            matrix< T, column_major >& c)
  {
    std::cout << "gemm a trans(b)" << std::endl;
    std::cout << "addres : " << &b(0,0) << std::endl;
    assert( a.size1() == c.size1() ); // m
    assert( b.size2() == c.size2() ); // n
    assert( a.size2() == b.size1() ); // k
    const int m = a.size1(); 
    const int n = b.size2();
    const int k = a.size2();
    const int lda = m;
    const int ldb = n;
    const int ldc = m;
    blas_traits< T >::gemm( &TRANS_N, &TRANS_T, &m, &n, &k, &alpha, &(a.data()[0]), &lda, &(b.data()[0]), &ldb, &beta, &(c.data()[0]), &ldc );
  }
}}

#endif // boost_ublas_blas_hpp
