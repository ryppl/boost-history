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

#ifndef BOOST_NUMERIC_CBLAS_SYMM_HERM_TRAITS_HPP
#define BOOST_NUMERIC_CBLAS_SYMM_HERM_TRAITS_HPP

#include <complex> 
#include <boost/numeric/bindings/traits/traits.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  namespace detail {

    // select symmetric or hermitian traits 

    struct meta_symm_adaptor {
      template <typename M> 
      struct traits { typedef symmetric_adaptor_traits<M> type; }; 
    };
    struct meta_herm_adaptor {
      template <typename M> 
      struct traits { typedef hermitian_adaptor_traits<M> type; }; 
    };

    template <typename ScalarT, typename M> 
    struct symm_herm_adaptor_traits {}; 
    template <typename M>
    struct symm_herm_adaptor_traits<float, M> {
      typedef typename meta_symm_adaptor::template traits<M>::type type; 
    }; 
    template <typename M>
    struct symm_herm_adaptor_traits<double, M> {
      typedef typename meta_symm_adaptor::template traits<M>::type type; 
    }; 
    template <typename M>
    struct symm_herm_adaptor_traits<std::complex<float>, M> {
      typedef typename meta_herm_adaptor::template traits<M>::type type; 
    }; 
    template <typename M>
    struct symm_herm_adaptor_traits<std::complex<double>, M> {
      typedef typename meta_herm_adaptor::template traits<M>::type type; 
    }; 

  }

}}}}

#endif 
