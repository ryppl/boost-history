// (-*- C++ -*- header)  

/*
 * 
 * Copyright (c) Kresimir Fresl 2002 
 *
 * Permission to copy, modify, use and distribute this software 
 * for any non-commercial or commercial purpose is granted provided 
 * that this license appear on all copies of the software source code.
 *
 * Author assumes no responsibility whatsoever for its use and makes 
 * no guarantees about its quality, correctness or reliability.
 *
 * Author acknowledges the support of the Faculty of Civil Engineering, 
 * Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_CBLAS_STORAGE_ORDERING_HPP
#define BOOST_NUMERIC_CBLAS_STORAGE_ORDERING_HPP

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/cblas/cblas_inc.h>

namespace boost { namespace numeric { namespace bindings { 

  namespace cblas {

    template <typename O>
    struct storage_order {};
    template<> struct storage_order<traits::row_major_t> {
      enum { value = CblasRowMajor };
    };
    template<> struct storage_order<traits::column_major_t> {
      enum { value = CblasColMajor };
    };

  }

}}}

#endif 
