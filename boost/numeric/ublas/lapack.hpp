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

  void LAPACK_DGEEV(char* jobvl, char* jobvr, int * n, double*   a, int* lda, double*   wr, double*   wi, double*   vl, int* ldvl, double*   vr, int* ldvr, double*   work, int* lwork, int* info);
  void LAPACK_ZGEEV(char* jobvl, char* jobvr, int * n, dcomplex* a, int* lda, dcomplex* wr, dcomplex* wi, dcomplex* vl, int* ldvl, dcomplex* vr, int* ldvr, dcomplex* work, int* lwork, int* info);

  void LAPACK_DGEES(char* jobvl, char* sort, fortran_function_type select, int* n, double*   a, int* lda, int* sdim, double*   wr, double*   wi, double*   vs, int* ldvs, double*   work, int* lwork, int* bwork, int* info);
  void LAPACK_ZGEES(char* jobvl, char* sort, fortran_function_type select, int* n, dcomplex* a, int* lda, int* sdim, dcomplex* wr, dcomplex* wi, dcomplex* vs, int* ldvs, dcomplex* work, int* lwork, int* bwork, int* info);
}


namespace boost { namespace numerics {

  template < typename T >
  struct lapack_traits
  {
    typedef void (*getrf_type)(int* m, int* n, T* a, int* lda, int* ipiv, int* info) ;
    typedef void (*getrs_type)(char* trans, int *n, int* nrhs, T* a, int* lda, int* ipiv, T* b, int* ldb, int* info);
    // typedef void (*sytrs_type)(char* trans, int *n, int* nrhs, T* a, int* lda, int* ipiv, T* b, int* ldb, int* info);
    typedef void (*geev_type)(char* jobvl, char* jobvr, int * n, T* a, int* lda, T* wr, T* wi, T* vl, int* ldvl, T* vr, int* ldvr, T* work, int* lwork, int* info);
    typedef void (*gees_type)(char* jobvl, char* sort, fortran_function_type select, int* n, T* a, int* lda, int* sdim, T*  wr, T* wi, T* vs, int* ldvs, T* work, int* lwork, int* bwork, int* info);

    static getrf_type getrf ;
    static getrs_type getrs ;
    // static sytrs_type sytrs ;
    static geev_type geev ;
    static gees_type gees ;
  };

  extern double* work_buffer ;
  extern int work_buffer_size ;

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

  template < typename T, typename VectorT >
  int geev(char jobvl, char jobvr, matrix< T, column_major >& a, VectorT& wr, VectorT& wi, matrix< T, column_major >& vl, matrix< T, column_major >& vr) 
  {
    assert( jobvl == 'N' || jobvl == 'V' );
    assert( jobvr == 'N' || jobvr == 'V' );
    assert( a.size1() == a.size2() );
    int n = a.size2();
    int lda = a.size1();
    int ldvl = vl.size1();
    int ldvr = vr.size1();
    int lwork = 8 * n;
    if ( lwork > work_buffer_size ) {
      delete[] work_buffer ;
      work_buffer = new double[ lwork ];
      work_buffer_size = lwork ;
    }
    int info = 0;
    lapack_traits< T >::geev( &jobvl, &jobvr, &n, &(a.data()[0]), &lda, &wr[0], &wi[0], &(vl.data()[0]), &ldvl, &(vr.data()[0]), &ldvr, work_buffer, &work_buffer_size, &info );
    return info;
  }

  template < typename T, typename VectorT >
  void gees(char jobvl, char sort, fortran_function_type select, matrix< T, column_major >& a, int& sdim, VectorT& wr, VectorT& wi, matrix< T, column_major >& vs)
  {
    assert( a.size1() == a.size2() );
    int n = a.size2();
    int ldvs = vs.size2();
    if ( work_buffer_size < 6 * n ) {
      buffer_work_size = 6 * n;
      delete[] buffer_work;
      buffer_work = new T[ buffer_work_size ];
    }
    if ( bwork_buffer_size < n ) {
      bwork_buffer_size = n;
      delete[] bwork_buffer;
      bwork_buffer = new T[ bwork_buffer_size ];
    }
    int info = 0;
    lapack_traits< T >::gees( &jobvl, &sort, select, &(a.data()[0]), &lda, &sdim, &wr[0], &wi[0], &(vs.data()[0]), work_buffer, &work_buffer_size, bwork_buffer, &info );
    return info;
  }
}}

#endif // boost_ublas_lapack_hpp
