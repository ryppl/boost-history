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

#ifndef BOOST_NUMERIC_CBLAS_LEVEL_3_HPP
#define BOOST_NUMERIC_CBLAS_LEVEL_3_HPP

#include <cassert>

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/cblas/cblas3_overloads.hpp>
#include <boost/numeric/bindings/cblas/storage_order.hpp>

namespace boost { namespace numeric { namespace bindings { 

  namespace cblas {

    // C <- alpha * op (A) * op (B) + beta * C 
    // op (A) == A || A^T || A^H
    template < 
      typename T, typename matrix_a_t, 
      typename matrix_b_t, typename matrix_c_t
      >
    void gemm (CBLAS_TRANSPOSE const TransA, CBLAS_TRANSPOSE const TransB, 
	       T const alpha, matrix_a_t const& a, matrix_b_t const& b, 
	       T const beta, matrix_c_t& c
	       )
    {
      typedef traits::matrix_traits<const matrix_a_t> matraits ; 
      typedef traits::matrix_traits<const matrix_b_t> mbtraits ; 
      typedef traits::matrix_traits<matrix_c_t> mctraits ; 

      const int m = TransA == CblasNoTrans
	? matraits::size1 (a)
	: matraits::size2 (a);
      const int n = TransB == CblasNoTrans
	? mbtraits::size2 (b)
	: mbtraits::size1 (b);
      const int k = TransA == CblasNoTrans
	? matraits::size2 (a)
	: matraits::size1 (a); 
#ifndef NDEBUG
      const int k1 = TransB == CblasNoTrans
	? mbtraits::size1 (b)
	: mbtraits::size2 (b);
#endif
      assert (m == mctraits::size1 (c)); 
      assert (n == mctraits::size2 (c)); 
      assert (k == k1); 
      // .. what about AtlasConj? 

      CBLAS_ORDER const ord_a
	= static_cast<CBLAS_ORDER const>
	(storage_order<typename matraits::ordering_type>::value); 
#ifndef NDEBUG
      CBLAS_ORDER const ord_b
	= static_cast<CBLAS_ORDER const>
	(storage_order<typename matraits::ordering_type>::value); 
      CBLAS_ORDER const ord_c
	= static_cast<CBLAS_ORDER const>
	(storage_order<typename matraits::ordering_type>::value); 
      assert (ord_b == ord_a);
      assert (ord_c == ord_a);
#endif 
      
      detail::gemm (ord_a, TransA, TransB, m, n, k, alpha, 
		    matraits::storage (a), matraits::leading_dimension (a),
		    mbtraits::storage (b), mbtraits::leading_dimension (b),
		    beta, 
		    mctraits::storage (c), mctraits::leading_dimension (c)); 
    }


    // C <- alpha * A * B + beta * C 
    template < 
      typename T, typename matrix_a_t, 
      typename matrix_b_t, typename matrix_c_t
      >
    void gemm (T const alpha, matrix_a_t const& a, matrix_b_t const& b, 
	       T const beta, matrix_c_t& c) {
      gemm (CblasNoTrans, CblasNoTrans, alpha, a, b, beta, c) ;
    }
    

    // C <- A * B 
    template <typename matrix_a_t, typename matrix_b_t, typename matrix_c_t>
    void gemm (matrix_a_t const& a, matrix_b_t const& b, matrix_c_t& c)
    {
      typedef typename traits::matrix_traits<matrix_c_t>::value_type val_t; 
      gemm (CblasNoTrans, CblasNoTrans, (val_t) 1, a, b, (val_t) 0, c);
    }

  } // namespace cblas

}}} 

#endif // BOOST_NUMERIC_CBLAS_LEVEL_3_HPP
