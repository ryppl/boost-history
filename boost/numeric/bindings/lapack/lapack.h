#ifndef boost_numeric_bindings_lapack_lapack_h
#define boost_numeric_bindings_lapack_lapack_h

#ifdef __cplusplus
#ifndef boost_numeric_bindings_lapack_traits_hpp
#error include lapack.hpp in C++ instead
#endif
#endif

#include <boost/numeric/bindings/traits/fortran.h>

#if defined (BIND_FORTRAN_LOWERCASE_UNDERSCORE)

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

#define LAPACK_DPOTRF dpotrf_
#define LAPACK_ZPOTRF zpotrf_

#define LAPACK_DPOTRS dpotrs_
#define LAPACK_ZPOTRS zpotrs_

#elif defined(BIND_FORTRAN_LOWERCASE)

#define LAPACK_DGETRF dgetrf
#define LAPACK_ZGETRF zgetrf

#define LAPACK_DGETRS dgetrs
#define LAPACK_ZGETRS zgetrs

#define LAPACK_DSYTRF dsytrf
#define LAPACK_ZSYTRF zsytrf

#define LAPACK_DSYTRS dsytrs
#define LAPACK_ZSYTRS zsytrs

#define LAPACK_DGEEV dgeev
#define LAPACK_ZGEEV zgeev

#define LAPACK_DGEES dgees
#define LAPACK_ZGEES zgees

#define LAPACK_DPOTRF dpotrf
#define LAPACK_ZPOTRF zpotrf

#define LAPACK_DPOTRS dpotrs
#define LAPACK_ZPOTRS zpotrs

#else
#error do not know how to bind to fortran calling convention
#endif

#include <boost/numeric/bindings/traits/type.h>

extern "C"
{
  typedef int (*fortran_function_type)(...) ;

  void LAPACK_DGETRF(const int* m, const int* n, double*   a, const int* lda, int* ipiv, int* info) ;
  void LAPACK_ZGETRF(const int* m, const int* n, dcomplex* a, const int* lda, int* ipiv, int* info) ;

  void LAPACK_DGETRS(const char* trans, const int *n, const int* nrhs, const double*   a, const int* lda, const int* ipiv, double*   b, const int* ldb, int* info);
  void LAPACK_ZGETRS(const char* trans, const int *n, const int* nrhs, const dcomplex* a, const int* lda, const int* ipiv, dcomplex* b, const int* ldb, int* info);

  void LAPACK_DSYTRF(const char* uplo, const int* n, double*   a, const int* lda, int* ipiv, double* work, const int* lwork, int* info) ;
  void LAPACK_ZSYTRF(const char* uplo, const int* n, dcomplex* a, const int* lda, int* ipiv, dcomplex* work, const int* lwork, int* info) ;

  void LAPACK_DSYTRS(const char* uplo, const int *n, const int* nrhs, const double*   a, const int* lda, const int* ipiv, double*               b, const int* ldb, int* info);
  void LAPACK_ZSYTRS(const char* uplo, const int *n, const int* nrhs, const dcomplex* a, const int* lda, const int* ipiv, dcomplex* b, const int* ldb, int* info);

  void LAPACK_DGEEV(const char* jobvl, const char* jobvr, const int * n, double*   a, const int* lda, double* wr, double* wi, double* vl, const int* ldvl, double* vr, const int* ldvr, double* work, const int* lwork, int* info);
  void LAPACK_ZGEEV(const char* jobvl, const char* jobvr, const int * n, dcomplex* a, const int* lda, dcomplex* w, dcomplex* vl, const int* ldvl, dcomplex* vr, const int* ldvr, dcomplex* work, const int* lwork, double* rwork, int* info);

  void LAPACK_DPOTRF(const char* uplo, const int * n, double* a, const int* lda, int* info);
  void LAPACK_ZPOTRF(const char* uplo, const int * n, dcomplex* a, const int* lda, int* info);

  void LAPACK_DPOTRS(const char* uplo, const int * n, const int * nrhs, const double* a, const int* lda, double* b, const int* ldb, int* info);
  void LAPACK_ZPOTRS(const char* uplo, const int * n, const int * nrhs, const dcomplex* a, const int* lda, dcomplex* b, const int* ldb, int* info);

  // void LAPACK_DGEES(const char* jobvl, const char* sort, fortran_function_type select, const int* n, double*   a, const int* lda, int* sdim, double*   wr, double*   wi, double*   vs, const int* ldvs, double*   work, const int* lwork, int* bwork, int* info);
  // void LAPACK_ZGEES(const char* jobvl, const char* sort, fortran_function_type select, const int* n, dcomplex* a, const int* lda, int* sdim, dcomplex* w, dcomplex* vs, int* ldvs, dcomplex* work, const int* lwork, double* rwork, int* bwork, int* info);
}
#endif // boost_numeric_bindings_lapack_lapack_h
