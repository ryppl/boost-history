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

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_LAPACK_NAMES_H
#define BOOST_NUMERIC_BINDINGS_LAPACK_LAPACK_NAMES_H

#include <boost/numeric/bindings/traits/fortran.h>

// general

#define LAPACK_SGESV FORTRAN_ID( sgesv )
#define LAPACK_DGESV FORTRAN_ID( dgesv )
#define LAPACK_CGESV FORTRAN_ID( cgesv )
#define LAPACK_ZGESV FORTRAN_ID( zgesv )

#define LAPACK_SGETRF FORTRAN_ID( sgetrf )
#define LAPACK_DGETRF FORTRAN_ID( dgetrf )
#define LAPACK_CGETRF FORTRAN_ID( cgetrf )
#define LAPACK_ZGETRF FORTRAN_ID( zgetrf )

#define LAPACK_SGETRS FORTRAN_ID( sgetrs )
#define LAPACK_DGETRS FORTRAN_ID( dgetrs )
#define LAPACK_CGETRS FORTRAN_ID( cgetrs )
#define LAPACK_ZGETRS FORTRAN_ID( zgetrs )

#define LAPACK_SGETRI FORTRAN_ID( sgetri )
#define LAPACK_DGETRI FORTRAN_ID( dgetri )
#define LAPACK_CGETRI FORTRAN_ID( cgetri )
#define LAPACK_ZGETRI FORTRAN_ID( zgetri )


// symmetric/Hermitian positive definite 

#define LAPACK_SPOSV FORTRAN_ID( sposv )
#define LAPACK_DPOSV FORTRAN_ID( dposv )
#define LAPACK_CPOSV FORTRAN_ID( cposv )
#define LAPACK_ZPOSV FORTRAN_ID( zposv )

#define LAPACK_SPOTRF FORTRAN_ID( spotrf )
#define LAPACK_DPOTRF FORTRAN_ID( dpotrf )
#define LAPACK_CPOTRF FORTRAN_ID( cpotrf )
#define LAPACK_ZPOTRF FORTRAN_ID( zpotrf )

#define LAPACK_SPOTRS FORTRAN_ID( spotrs )
#define LAPACK_DPOTRS FORTRAN_ID( dpotrs )
#define LAPACK_CPOTRS FORTRAN_ID( cpotrs )
#define LAPACK_ZPOTRS FORTRAN_ID( zpotrs )

#define LAPACK_SPOTRI FORTRAN_ID( spotri )
#define LAPACK_DPOTRI FORTRAN_ID( dpotri )
#define LAPACK_CPOTRI FORTRAN_ID( cpotri )
#define LAPACK_ZPOTRI FORTRAN_ID( zpotri )


// symmetric/Hermitian indefinite 
// and complex symmetric 

#define LAPACK_SSYSV FORTRAN_ID( ssysv )
#define LAPACK_DSYSV FORTRAN_ID( dsysv )
#define LAPACK_CSYSV FORTRAN_ID( csysv )
#define LAPACK_ZSYSV FORTRAN_ID( zsysv )
#define LAPACK_CHESV FORTRAN_ID( chesv )
#define LAPACK_ZHESV FORTRAN_ID( zhesv )

#define LAPACK_SSYTRF FORTRAN_ID( ssytrf )
#define LAPACK_DSYTRF FORTRAN_ID( dsytrf )
#define LAPACK_CSYTRF FORTRAN_ID( csytrf )
#define LAPACK_ZSYTRF FORTRAN_ID( zsytrf )
#define LAPACK_CHETRF FORTRAN_ID( chetrf )
#define LAPACK_ZHETRF FORTRAN_ID( zhetrf )

#define LAPACK_SSYTRS FORTRAN_ID( ssytrs )
#define LAPACK_DSYTRS FORTRAN_ID( dsytrs )
#define LAPACK_CSYTRS FORTRAN_ID( csytrs )
#define LAPACK_ZSYTRS FORTRAN_ID( zsytrs )
#define LAPACK_CHETRS FORTRAN_ID( chetrs )
#define LAPACK_ZHETRS FORTRAN_ID( zhetrs )



/********************************************/

#define LAPACK_DGEEV FORTRAN_ID( dgeev )
#define LAPACK_ZGEEV FORTRAN_ID( zgeev )

#define LAPACK_DGEES FORTRAN_ID( dgees )
#define LAPACK_ZGEES FORTRAN_ID( zgees )


#endif 

