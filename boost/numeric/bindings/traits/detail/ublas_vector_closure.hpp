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

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_VECTOR_CLOSURE_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_VECTOR_CLOSURE_H

#include <boost/numeric/bindings/traits/detail/ublas_closure.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  namespace detail {

    // vector references 

    template <typename V>
    struct 
    closure_traits<boost::numeric::ublas::vector_reference<V> > {
      typedef boost::numeric::ublas::vector_reference<V> ref_type;
      typedef typename ref_type::expression_type type; 
      static type& get (ref_type& r) { 
	return r.expression();
      }
    };

    template <typename V>
    struct 
    closure_traits<boost::numeric::ublas::vector_reference<V> const> {
      typedef boost::numeric::ublas::vector_reference<V> const ref_type;
      typedef typename ref_type::expression_type const type; 
      static type& get (ref_type const& r) { 
	return r.expression();
      }
    };

  }

}}}}

#endif 
