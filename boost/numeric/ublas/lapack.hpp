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

#define LAPACK_DGEEV dgeev_
#define LAPACK_ZGEEV zgeev_

#define LAPACK_DGEES dgees_
#define LAPACK_ZGEES zgees_

#else
#error did not recognise compiler
#endif

#include <boost/numeric/ublas/matrix.h>
#include <complex>

extern "C" 
{
  typedef int (*fortran_function_type)(...);
  typedef std::complex< double > dcomplex ;

  void LAPACK_DGETRF(int* m, int* n, double*               a, int* lda, int* ipiv, int* info) ;
  void LAPACK_ZGETRF(int* m, int* n, std::complex<double>* a, int* lda, int* ipiv, int* info) ;

  void LAPACK_DGETRS(char* trans, int *n, int* nrhs, double*               a, int* lda, int* ipiv, double*               b, int* ldb, int* info);
  void LAPACK_ZGETRS(char* trans, int *n, int* nrhs, std::complex<double>* a, int* lda, int* ipiv, std::complex<double>* b, int* ldb, int* info);

  // void LAPACK_DSYTRS(char* trans, int *n, int* nrhs, double*               a, int* lda, int* ipiv, double*               b, int* ldb, int* info);
  // void LAPACK_ZSYTRS(char* trans, int *n, int* nrhs, std::complex<double>* a, int* lda, int* ipiv, std::complex<double>* b, int* ldb, int* info);

  void LAPACK_DGEEV(char* jobvl, char* jobvr, int * n, double*   a, int* lda, double* wr, double* wi, double* vl, int* ldvl, double* vr, int* ldvr, double* work, int* lwork, int* info);
  void LAPACK_ZGEEV(char* jobvl, char* jobvr, int * n, dcomplex* a, int* lda, dcomplex* w, dcomplex* vl, int* ldvl, dcomplex* vr, int* ldvr, dcomplex* work, int* lwork, double* rwork, int* info);

  void LAPACK_DGEES(char* jobvl, char* sort, fortran_function_type select, int* n, double*   a, int* lda, int* sdim, double*   wr, double*   wi, double*   vs, int* ldvs, double*   work, int* lwork, int* bwork, int* info);
  void LAPACK_ZGEES(char* jobvl, char* sort, fortran_function_type select, int* n, dcomplex* a, int* lda, int* sdim, dcomplex* w, dcomplex* vs, int* ldvs, dcomplex* work, int* lwork, double* rwork, int* bwork, int* info);
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

  extern double* work_buffer ;
  extern int work_buffer_size ;

  extern double* rwork_buffer ;
  extern int rwork_buffer_size ;

  extern int* bwork_buffer ;
  extern int bwork_buffer_size ;

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

  template < typename T >
  int geev(char jobvl, char jobvr, matrix< T, column_major >& a, vector< std::complex< double > >& w, matrix< T, column_major >& vl, matrix< T, column_major >& vr) 
  { assert( 0 ); return 0; }

  template <>
  int geev< double >(char jobvl, char jobvr, matrix< double, column_major >& a, vector< std::complex< double > >& w, matrix< double, column_major >& vl, matrix< double, column_major >& vr) ;

  template <>
  int geev< std::complex< double > >(char jobvl, char jobvr, matrix< std::complex< double >, column_major >& a, vector< std::complex< double > >& w, matrix< std::complex< double >, column_major >& vl, matrix< std::complex< double >, column_major >& vr) ;

  template < typename T >
  int gees(char jobvl, char sort, matrix< T, column_major >& a, int& sdim, vector< std::complex< double > >& w, matrix< T, column_major >& vs)
  { assert( 0 ); return 0; }

  template <>
  int gees< double >(char jobvl, char sort, matrix< double, column_major >& a, int& sdim, vector< std::complex< double > >& w, matrix< double, column_major >& vs) ;

  template <>
  int gees< std::complex< double > >(char jobvl, char sort, matrix< std::complex< double >, column_major >& a, int& sdim, vector< std::complex< double > >& w, matrix< std::complex< double >, column_major >& vs) ;
}}

#endif // boost_ublas_lapack_hpp
