//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright Toon Knapen

#ifndef boost_numeric_bindings_amos_amos_h
#define boost_numeric_bindings_amos_amos_h

#ifdef __cplusplus
#ifndef boost_numeric_bindings_amos_amos_hpp
#error "include amos.hpp instead"
#endif
#endif

#include <boost/numeric/bindings/traits/fortran.h>

#if defined(BIND_FORTRAN_LOWERCASE_UNDERSCORE)

#define AMOS_DBESI dbesi_
#define AMOS_CBESI cbesi_
#define AMOS_ZBESI zbesi_

#define AMOS_DBESJ dbesj_
#define AMOS_CBESJ cbesj_
#define AMOS_ZBESJ zbesj_

#define AMOS_DBESY dbesy_
#define AMOS_CBESY cbesy_
#define AMOS_ZBESY zbesy_

#elif defined( BIND_FORTRAN_LOWERCASE )

#define AMOS_DBESI dbesi
#define AMOS_CBESI cbesi
#define AMOS_ZBESI zbesi

#define AMOS_DBESJ dbesj
#define AMOS_CBESJ cbesj
#define AMOS_ZBESJ zbesj

#define AMOS_DBESY dbesy
#define AMOS_CBESY cbesy
#define AMOS_ZBESY zbesy

#else
#error do not how to link with fortran
#endif

#include <boost/numeric/bindings/traits/type.h>
extern "C"
{
  void AMOS_DBESI(const double*   z,                  const double* fnu, const int* kode, const int* n, double*   cy,             int* nz           ) ;
  void AMOS_CBESI(const fcomplex* z,                  const float* fnu,  const int* kode, const int* n, fcomplex* cy,             int* nz, int* ierr) ;
  void AMOS_ZBESI(const double* zr, const double* zi, const double* fnu, const int* kode, const int* n, double* cyr, double* cyi, int* nz, int* ierr) ;

  void AMOS_DBESJ(const double  * z,                  const double* fnu,                  const int* n, double*   cy,             int* nz           ) ;
  void AMOS_CBESJ(const fcomplex* z,                  const float* fnu,  const int* kode, const int* n, fcomplex* cy,             int* nz, int* ierr) ;
  void AMOS_ZBESJ(const double* zr, const double* zi, const double* fnu, const int* kode, const int* n, double* cyr, double* cyi, int* nz, int* ierr) ;

  void AMOS_DBESY(const double  * z,                  const double* fnu,                  const int* n, double*   cy                                                              ) ;
  void AMOS_CBESY(const fcomplex* z,                  const float* fnu,  const int* kode, const int* n, fcomplex* cy,             int* nz, fcomplex *cwrk,               int* ierr) ;
  void AMOS_ZBESY(const double* zr, const double* zi, const double* fnu, const int* kode, const int* n, double* cyr, double* cyi, int* nz, double *cwrkr, double *cwrki, int* ierr) ;
}

#endif // boost_numeric_bindings_amos_amos_hpp
