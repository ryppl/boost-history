#ifndef boost_ublas_lapack_hpp
#define boost_ublas_lapack_hpp

#if defined(__GNUC__) || defined(__ICC)
#define LAPACK_DGETRF dgetrf_
#define LAPACK_ZGETRF zgetrf_

#define LAPACK_DGETRS dgetrs_
#define LAPACK_ZGETRS zgetrs_

#define LAPACK_DSYTRF dsytrf_
#define LAPACK_ZSYTRF zsytrf_

#define LAPACK_DSYTRS dsytrs_
#define LAPACK_ZSYTRS zsytrs_
#else
#error did not recognise compiler
#endif

#include <boost/numeric/ublas/matrix.h>
#include <complex>

extern "C" 
{
  void LAPACK_DGETRF(int* m, int* n, double*               a, int* lda, int* ipiv, int* info) ;
  void LAPACK_ZGETRF(int* m, int* n, std::complex<double>* a, int* lda, int* ipiv, int* info) ;

  void LAPACK_DGETRS(char* trans, int *n, int* nrhs, double*               a, int* lda, int* ipiv, double*               b, int* ldb, int* info);
  void LAPACK_ZGETRS(char* trans, int *n, int* nrhs, std::complex<double>* a, int* lda, int* ipiv, std::complex<double>* b, int* ldb, int* info);

  // void LAPACK_DSYTRS(char* trans, int *n, int* nrhs, double*               a, int* lda, int* ipiv, double*               b, int* ldb, int* info);
  // void LAPACK_ZSYTRS(char* trans, int *n, int* nrhs, std::complex<double>* a, int* lda, int* ipiv, std::complex<double>* b, int* ldb, int* info);
}


namespace boost { namespace numerics {

  template < typename T >
  struct lapack_traits
  {
    typedef void (*getrf_type)(int* m, int* n, T* a, int* lda, int* ipiv, int* info) ;
    typedef void (*getrs_type)(char* trans, int *n, int* nrhs, T* a, int* lda, int* ipiv, T* b, int* ldb, int* info);
    // typedef void (*sytrs_type)(char* trans, int *n, int* nrhs, T* a, int* lda, int* ipiv, T* b, int* ldb, int* info);

    static getrf_type getrf ;
    static getrs_type getrs ;
    // static sytrs_type sytrs ;
  };

  template < typename T, typename IpivIterator >
  int getrf(matrix< T, column_major >& a, IpivIterator begin_ipiv, IpivIterator end_ipiv)
  {
    int m = a.size1();
    int n = a.size2();
    T* begin_a = &a(0,0);
    int lda = m ;
    assert( std::distance( begin_ipiv, end_ipiv ) >= std::min( m,  n ) );
    int info = 0;
    int& ipiv = *begin_ipiv ;
    lapack_traits< T >::getrf( &m, &n, begin_a, &lda, &ipiv, &info );
    return info;
  }

  template <typename T, typename storage_type, typename IntIterator >
  int getrs(char trans, matrix< T, storage_type >& a, IntIterator begin_ipiv, IntIterator end_ipiv, matrix< T, column_major >& b)
  {
    assert( trans == 'N' || trans == 'T' );
    int n = a.size1();
    int nrhs = b.size2();
    T* begin_a = &a(0,0);
    int lda = n;
    T* begin_b = &b(0,0); 
    int ldb = n;
    int& ipiv = *begin_ipiv ;
    int info = 0;
    assert( n == static_cast<int>(a.size2()) && n == static_cast<int>(b.size1()) );
    assert( std::distance( begin_ipiv, end_ipiv ) >= n );
    lapack_traits< T >::getrs( &trans, &n, &nrhs, begin_a, &lda, &ipiv, begin_b, &ldb, &info );
    return info ;
  }

}}

#endif // boost_ublas_lapack_hpp
