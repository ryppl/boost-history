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

#ifndef BOOST_NUMERIC_CBLAS_LEVEL_2_HPP
#define BOOST_NUMERIC_CBLAS_LEVEL_2_HPP

#include <cassert>

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/atlas/cblas2_overloads.hpp>
#include <boost/numeric/bindings/atlas/cblas_enum.hpp>

namespace boost { namespace numeric { namespace bindings { 

  namespace atlas {

    // y <- alpha * op (A) * x + beta * y
    // op (A) == A || A^T || A^H
    template <typename T, typename Matr, typename VctX, typename VctY>
    inline 
    void gemv (CBLAS_TRANSPOSE const TransA, 
	       T const alpha, Matr const& a, VctX const& x, 
	       T const beta, VctY& y
	       )
    {
      typedef traits::matrix_traits<Matr const> mtraits; 
      typedef traits::vector_traits<VctX const> xvtraits; 
      typedef traits::vector_traits<VctY> yvtraits; 

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
    template <typename T, typename Matr, typename VctX, typename VctY>
    inline 
    void gemv (T const alpha, Matr const& a, VctX const& x, 
	       T const beta, VctY& y) {
      gemv (CblasNoTrans, alpha, a, x, beta, y); 
    }
    

    // y <- A * x
    template <typename Matr, typename VctX, typename VctY>
    inline 
    void gemv (Matr const& a, VctX const& x, VctY& y) {
      typedef typename traits::matrix_traits<Matr>::value_type val_t; 
      gemv (CblasNoTrans, (val_t) 1, a, x, (val_t) 0, y);
    }

  } // namespace atlas

}}} 

#endif // BOOST_NUMERIC_CBLAS_LEVEL_2_HPP
