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

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_LAPACK_OR_CLAPACK_H
#define BOOST_NUMERIC_BINDINGS_LAPACK_LAPACK_OR_CLAPACK_H

#ifndef BOOST_NUMERIC_BINDINGS_CLAPACK

#include <boost/numeric/bindings/lapack/lapack.h>

namespace boost { namespace numeric { namespace bindings { namespace lapack {

  typedef int int_t; 

  // maybe_remove_const(): CLAPACK is not const correct
  template <typename T>
  inline T const* mrc (T const* t) { return t; }

}}}}

#else // BOOST_NUMERIC_BINDINGS_CLAPACK

#define FORTRAN_ID( id ) id##_
#include <boost/numeric/bindings/lapack/lapack_names.h>

#include <boost/numeric/bindings/traits/f2c.h>
#include <boost/numeric/bindings/lapack/clapack_inc.h>

namespace boost { namespace numeric { namespace bindings { namespace lapack {

  typedef ::integer int_t; 

  // maybe_remove_const(): CLAPACK is not const correct
  template <typename T>
  inline T* mrc (T const* t) { return const_cast<T*> (t); }

}}}}

#endif // BOOST_NUMERIC_BINDINGS_CLAPACK

#endif // BOOST_NUMERIC_BINDINGS_LAPACK_LAPACK_OR_CLAPACK_H

