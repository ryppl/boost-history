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

#ifndef BOOST_NUMERIC_CBLAS_LEVEL_2_HPP
#define BOOST_NUMERIC_CBLAS_LEVEL_2_HPP

#include <cassert>

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/cblas/cblas2_overloads.hpp>
#include <boost/numeric/bindings/cblas/storage_order.hpp>

namespace boost { namespace numeric { namespace bindings { 

  namespace cblas {

    // y <- alpha * op (A) * x + beta * y
    // op (A) == A || A^T || A^H
    template < 
      typename T, typename matrix_t, typename vector_x_t, typename vector_y_t 
      >
    void gemv (CBLAS_TRANSPOSE const TransA, 
	       T const alpha, matrix_t const& a, vector_x_t const& x, 
	       T const beta, vector_y_t& y
	       )
    {
      typedef traits::matrix_traits<matrix_t const> mtraits ; 
      typedef traits::vector_traits<vector_x_t const> xvtraits ; 
      typedef traits::vector_traits<vector_y_t> yvtraits ; 

      int const m = mtraits::size1 (a);
      int const n = mtraits::size2 (a);
      assert (xvtraits::size (x) >= (TransA == CblasNoTrans ? n : m)); 
      assert (yvtraits::size (y) >= (TransA == CblasNoTrans ? m : n)); 
      // .. what about AtlasConj? 

      CBLAS_ORDER const stor_ord
	= static_cast<CBLAS_ORDER const>
	(storage_order<typename mtraits::ordering_type>::value); 

      detail::gemv (stor_ord, TransA, m, n, alpha, mtraits::storage (a), 
		    mtraits::leading_dimension (a),
		    xvtraits::storage (x), xvtraits::stride (x),
		    beta, yvtraits::storage (y), yvtraits::stride (y));
    }


    // y <- alpha * A * x + beta * y 
    template < 
      typename T, typename matrix_t, typename vector_x_t, typename vector_y_t 
      >
    inline 
    void gemv (T const alpha, matrix_t const& a, vector_x_t const& x, 
	       T const beta, vector_y_t& y) {
      gemv (CblasNoTrans, alpha, a, x, beta, y); 
    }
    
    
    // y <- A * x
    template < 
      typename matrix_t, typename vector_x_t, typename vector_y_t 
      >
    inline 
    void gemv (matrix_t const& a, vector_x_t const& x, vector_y_t& y) {
      typedef typename traits::matrix_traits<matrix_t>::value_type val_t; 
      gemv (CblasNoTrans, (val_t) 1, a, x, (val_t) 0, y);
    }

  } // namespace cblas

}}} 

#endif // BOOST_NUMERIC_CBLAS_LEVEL_2_HPP
