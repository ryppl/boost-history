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

#else
#error do not know how to bind to fortran calling convention
#endif

#include <boost/numeric/bindings/traits/type.h>

extern "C"
{
  typedef int (*fortran_function_type)(...) ;

  void LAPACK_DGETRF(int* m, int* n, double*   a, int* lda, int* ipiv, int* info) ;
  void LAPACK_ZGETRF(int* m, int* n, dcomplex* a, int* lda, int* ipiv, int* info) ;

  void LAPACK_DGETRS(char* trans, int *n, int* nrhs, double*   a, int* lda, int* ipiv, double*   b, int* ldb, int* info);
  void LAPACK_ZGETRS(char* trans, int *n, int* nrhs, dcomplex* a, int* lda, int* ipiv, dcomplex* b, int* ldb, int* info);

  // void LAPACK_DSYTRS(char* trans, int *n, int* nrhs, double*               a, int* lda, int* ipiv, double*               b, int* ldb, int* info);
  // void LAPACK_ZSYTRS(char* trans, int *n, int* nrhs, std::complex<double>* a, int* lda, int* ipiv, std::complex<double>* b, int* ldb, int* info);

  void LAPACK_DGEEV(char* jobvl, char* jobvr, int * n, double*   a, int* lda, double* wr, double* wi, double* vl, int* ldvl, double* vr, int* ldvr, double* work, int* lwork, int* info);
  void LAPACK_ZGEEV(char* jobvl, char* jobvr, int * n, dcomplex* a, int* lda, dcomplex* w, dcomplex* vl, int* ldvl, dcomplex* vr, int* ldvr, dcomplex* work, int* lwork, double* rwork, int* info);

  // void LAPACK_DGEES(char* jobvl, char* sort, fortran_function_type select, int* n, double*   a, int* lda, int* sdim, double*   wr, double*   wi, double*   vs, int* ldvs, double*   work, int* lwork, int* bwork, int* info);
  // void LAPACK_ZGEES(char* jobvl, char* sort, fortran_function_type select, int* n, dcomplex* a, int* lda, int* sdim, dcomplex* w, dcomplex* vs, int* ldvs, dcomplex* work, int* lwork, double* rwork, int* bwork, int* info);
}
#endif // boost_numeric_bindings_lapack_lapack_h
