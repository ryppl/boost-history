/*
 * 
 * Copyright (c) Kresimir Fresl and Toon Knapen 2002, 2003 
 *
 * Permission to copy, modify, use and distribute this software 
 * for any non-commercial or commercial purpose is granted provided 
 * that this license appear on all copies of the software source code.
 *
 * Author assumes no responsibility whatsoever for its use and makes 
 * no guarantees about its quality, correctness or reliability.
 *
 * First author acknowledges the support of the Faculty of Civil Engineering, 
 * University of Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_TYPE_TRAITS_HPP
#define BOOST_NUMERIC_BINDINGS_TRAITS_TYPE_TRAITS_HPP

namespace boost { namespace numeric { namespace bindings { namespace traits {

  template <typename Real> 
  struct type_traits { 
    typedef Real type; 
    typedef Real real_type; 
  };

  template <typename Real> 
  struct type_traits<std::complex<Real> > { 
    typedef std::complex<Real> type; 
    typedef Real real_type; 
  };

  const float*  real_cast(const std::complex< float  >* c) { return (const float* )( c ) ; }
  const double* real_cast(const std::complex< double >* c) { return (const double*)( c ) ; }

  float*  real_cast(std::complex< float  >* c) { return (float* )( c ) ; }
  double* real_cast(std::complex< double >* c) { return (double*)( c ) ; }

}}}}

#endif // BOOST_NUMERIC_BINDINGS_TRAITS_TYPE_TRAITS_HPP

