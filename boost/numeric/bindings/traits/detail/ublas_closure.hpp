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
 * University of Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_CLOSURE_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_CLOSURE_H

#include <boost/type_traits.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  namespace detail {

    template <typename VMIdentifier, typename VM>
    struct closure_detail_traits {
      typedef VM type; 
      static VM& get (VM& vm) { return vm; } 
    };

    template <typename VM> 
    struct closure_traits : closure_detail_traits< typename boost::remove_const<VM>::type, VM > { }; 

  }

}}}}

#endif 
