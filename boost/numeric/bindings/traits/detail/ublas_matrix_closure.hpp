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

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_MATRIX_CLOSURE_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_MATRIX_CLOSURE_H

#include <boost/numeric/bindings/traits/detail/ublas_closure.hpp>
#include <boost/numeric/bindings/traits/detail/generate_const.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  namespace detail {

    // matrix references 

    template <typename M, typename MR>
    struct
    closure_detail_traits< boost::numeric::ublas::matrix_reference<M>, MR > {
      typedef MR                                                                   ref_type;
      typedef typename generate_const<MR,typename ref_type::expression_type>::type type; 
      static type& get (ref_type& r) { 
	return r.expression();
      }
    };

  }

}}}}

#endif 
