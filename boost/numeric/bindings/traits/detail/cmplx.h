
/*
 * 
 * Copyright (c) Kresimir Fresl 2003
 *
 * Permission to copy, modify, use and distribute this software 
 * for any non-commercial or commercial purpose is granted provided 
 * that this license appear on all copies of the software source code.
 *
 * Author assumes no responsibility whatsoever for its use and makes 
 * no guarantees about its quality, correctness or reliability.
 *
 * Author acknowledges the support of the Faculty of Civil Engineering, 
 * University of Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_CMPLX_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_CMPLX_H

typedef 
union { 
  float cmplx[2];
  double align_struct_;
} fcomplex_t;
  
typedef 
struct { 
  double cmplx[2];
} dcomplex_t;

#endif 
