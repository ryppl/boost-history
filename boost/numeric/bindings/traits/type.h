/*
 * Copyright (C) 2000,2001,2002,2003 Si-Lab b.v.b.a. and Toon Knapen
 * 
 * License is hereby granted to use and modify this software 
 * as long as this copyright notice is retained and modifications
 * are clearly marked.
 * This License does not permit you to redistribute this software.
 * 
 */

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_TYPE_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_TYPE_H

/*
 * This header defines the C types that will be mapped to
 * COMPLEX and COMPLEX*16 of Fortran
 */

#if defined(BOOST_NUMERIC_BINDINGS_USE_COMPLEX_STRUCT)

typedef
union {
  float cmplx[2] ;
  double align_struct_ ;
} fcomplex_t ;

typedef 
struct
  double cmplx[2] ;
} dcomplex_t ;

#elif defined(BOOST_NUMERIC_BINDINGS_CLAPACK)

#include <boost/numeric/bindings/traits/f2c.h> 

typedef complex fcomplex_t; 
typedef doublecomplex dcomplex_t; 

#else

typedef float  fcomplex_t ;
typedef double dcomplex_t ;

#endif /* BOOST_NUMERIC_BINDINGS_USE_COMPLEX_STRUCT */

#endif /* BOOST_NUMERIC_BINDINGS_TRAITS_TYPE_H */
