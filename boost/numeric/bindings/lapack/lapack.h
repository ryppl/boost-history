/*
 * 
 * Copyright (c) Toon Knapen & Kresimir Fresl 2003
 *
 * Permission to copy, modify, use and distribute this software 
 * for any non-commercial or commercial purpose is granted provided 
 * that this license appear on all copies of the software source code.
 *
 * Authors assume no responsibility whatsoever for its use and makes 
 * no guarantees about its quality, correctness or reliability.
 *
 * KF acknowledges the support of the Faculty of Civil Engineering, 
 * University of Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_LAPACK_H
#define BOOST_NUMERIC_BINDINGS_LAPACK_LAPACK_H

#include <boost/numeric/bindings/traits/type.h>
#include <boost/numeric/bindings/lapack/lapack_names.h>


extern "C" {

  void LAPACK_SGESV (int const* n, int const* nrhs, 
                     float* a, int const* lda, int* ipiv, 
                     float* b, int const* ldb, int* info);
  void LAPACK_DGESV (int const* n, int const* nrhs, 
                     double* a, int const* lda, int* ipiv, 
                     double* b, int const* ldb, int* info);
  void LAPACK_CGESV (int const* n, int const* nrhs,
                     fcomplex_t* a, int const* lda, int* ipiv, 
                     fcomplex_t* b, int const* ldb, int* info);
  void LAPACK_ZGESV (int const* n, int const* nrhs,
                     dcomplex_t* a, int const* lda, int* ipiv, 
                     dcomplex_t* b, int const* ldb, int* info);

  void LAPACK_SGETRF (int const* n, int const* nrhs, 
                      float* a, int const* lda, int* ipiv, int* info);
  void LAPACK_DGETRF (int const* n, int const* nrhs, 
                      double* a, int const* lda, int* ipiv, int* info);
  void LAPACK_CGETRF (int const* n, int const* nrhs,
                      fcomplex_t* a, int const* lda,
                      int* ipiv, int* info);
  void LAPACK_ZGETRF (int const* n, int const* nrhs,
                      dcomplex_t* a, int const* lda,
                      int* ipiv, int* info);

  void LAPACK_SGETRS (char const* trans, int const* n, int const* nrhs, 
                      float const* a, int const* lda, int const* ipiv, 
                      float* b, int const* ldb, int* info);
  void LAPACK_DGETRS (char const* trans, int const* n, int const* nrhs, 
                      double const* a, int const* lda, int const* ipiv, 
                      double* b, int const* ldb, int* info);
  void LAPACK_CGETRS (char const* trans, int const* n, int const* nrhs,
                      fcomplex_t const* a, int const* lda, int const* ipiv, 
                      fcomplex_t* b, int const* ldb, int* info);
  void LAPACK_ZGETRS (char const* trans, int const* n, int const* nrhs,
                      dcomplex_t const* a, int const* lda, int const* ipiv, 
                      dcomplex_t* b, int const* ldb, int* info);


  void LAPACK_SPOSV (char const* uplo, int const* n, int const* nrhs, 
                     float* a, int const* lda, 
                     float* b, int const* ldb, int* info);
  void LAPACK_DPOSV (char const* uplo, int const* n, int const* nrhs, 
                     double* a, int const* lda, 
                     double* b, int const* ldb, int* info);
  void LAPACK_CPOSV (char const* uplo, int const* n, int const* nrhs,
                     fcomplex_t* a, int const* lda,
                     fcomplex_t* b, int const* ldb, int* info);
  void LAPACK_ZPOSV (char const* uplo, int const* n, int const* nrhs,
                     dcomplex_t* a, int const* lda,
                     dcomplex_t* b, int const* ldb, int* info);

  void LAPACK_SPOTRF (char const* uplo, int const* n, 
                      float* a, int const* lda, int* info);
  void LAPACK_DPOTRF (char const* uplo, int const* n, 
                      double* a, int const* lda, int* info);
  void LAPACK_CPOTRF (char const* uplo, int const* n, 
                      fcomplex_t* a, int const* lda, int* info);
  void LAPACK_ZPOTRF (char const* uplo, int const* n, 
                      dcomplex_t* a, int const* lda, int* info);

  void LAPACK_SPOTRS (char const* uplo, int const* n, int const* nrhs, 
                      float const* a, int const* lda, 
                      float* b, int const* ldb, int* info);
  void LAPACK_DPOTRS (char const* uplo, int const* n, int const* nrhs, 
                      double const* a, int const* lda, 
                      double* b, int const* ldb, int* info);
  void LAPACK_CPOTRS (char const* uplo, int const* n, int const* nrhs,
                      fcomplex_t const* a, int const* lda,
                      fcomplex_t* b, int const* ldb, int* info);
  void LAPACK_ZPOTRS (char const* uplo, int const* n, int const* nrhs,
                      dcomplex_t const* a, int const* lda,
                      dcomplex_t* b, int const* ldb, int* info);


  void LAPACK_SSYSV (char const* uplo, int const* n, int const* nrhs, 
                     float* a, int const* lda, int* ipiv, 
                     float* b, int const* ldb, 
                     float* w, int const* lw, int* info);
  void LAPACK_DSYSV (char const* uplo, int const* n, int const* nrhs, 
                     double* a, int const* lda, int* ipiv, 
                     double* b, int const* ldb, 
                     double* w, int const* lw, int* info);
  void LAPACK_CSYSV (char const* uplo, int const* n, int const* nrhs,
                     fcomplex_t* a, int const* lda, int* ipiv, 
                     fcomplex_t* b, int const* ldb, 
                     fcomplex_t* w, int const* lw, int* info);
  void LAPACK_ZSYSV (char const* uplo, int const* n, int const* nrhs,
                     dcomplex_t* a, int const* lda, int* ipiv, 
                     dcomplex_t* b, int const* ldb, 
                     dcomplex_t* w, int const* lw, int* info);

  void LAPACK_CHESV (char const* uplo, int const* n, int const* nrhs,
                     fcomplex_t* a, int const* lda, int* ipiv, 
                     fcomplex_t* b, int const* ldb, 
                     fcomplex_t* w, int const* lw, int* info);
  void LAPACK_ZHESV (char const* uplo, int const* n, int const* nrhs,
                     dcomplex_t* a, int const* lda, int* ipiv, 
                     dcomplex_t* b, int const* ldb, 
                     dcomplex_t* w, int const* lw, int* info);

  void LAPACK_SSYTRF (char const* uplo, int const* n, 
                      float* a, int const* lda, int* ipiv, 
                      float* w, int const* lw, int* info);
  void LAPACK_DSYTRF (char const* uplo, int const* n, 
                      double* a, int const* lda, int* ipiv, 
                      double* w, int const* lw, int* info);
  void LAPACK_CSYTRF (char const* uplo, int const* n, 
                      fcomplex_t* a, int const* lda, int* ipiv, 
                      fcomplex_t* w, int const* lw, int* info);
  void LAPACK_ZSYTRF (char const* uplo, int const* n, 
                      dcomplex_t* a, int const* lda, int* ipiv, 
                      dcomplex_t* w, int const* lw, int* info);

  void LAPACK_CHETRF (char const* uplo, int const* n, 
                      fcomplex_t* a, int const* lda, int* ipiv, 
                      fcomplex_t* w, int const* lw, int* info);
  void LAPACK_ZHETRF (char const* uplo, int const* n, 
                      dcomplex_t* a, int const* lda, int* ipiv, 
                      dcomplex_t* w, int const* lw, int* info);

  void LAPACK_SSYTRS (char const* uplo, int const* n, int const* nrhs, 
                      float const* a, int const* lda, int const* ipiv, 
                      float* b, int const* ldb, int* info);
  void LAPACK_DSYTRS (char const* uplo, int const* n, int const* nrhs, 
                      double const* a, int const* lda, int const* ipiv, 
                      double* b, int const* ldb, int* info);
  void LAPACK_CSYTRS (char const* uplo, int const* n, int const* nrhs,
                      fcomplex_t const* a, int const* lda, int const* ipiv, 
                      fcomplex_t* b, int const* ldb, int* info);
  void LAPACK_ZSYTRS (char const* uplo, int const* n, int const* nrhs,
                      dcomplex_t const* a, int const* lda, int const* ipiv, 
                      dcomplex_t* b, int const* ldb, int* info);

  void LAPACK_CHETRS (char const* uplo, int const* n, int const* nrhs,
                      fcomplex_t const* a, int const* lda, int const* ipiv, 
                      fcomplex_t* b, int const* ldb, int* info);
  void LAPACK_ZHETRS (char const* uplo, int const* n, int const* nrhs,
                      dcomplex_t const* a, int const* lda, int const* ipiv, 
                      dcomplex_t* b, int const* ldb, int* info);

}

#endif 
