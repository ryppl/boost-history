// (-*- C++ -*- header)  

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

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_DETAIL_UTILS_HPP
#define BOOST_NUMERIC_BINDINGS_TRAITS_DETAIL_UTILS_HPP

#include <complex> 

namespace boost { namespace numeric { namespace bindings { namespace traits {

  namespace detail {

    inline int to_int (float f) { return static_cast<int> (f); }
    inline int to_int (double d) { return static_cast<int> (d); }
    inline int to_int (std::complex<float> cf) { 
      return static_cast<int> (cf.real()); 
    }
    inline int to_int (std::complex<double> cd) { 
      return static_cast<int> (cd.real()); 
    }

  }

}}}}

#endif 
