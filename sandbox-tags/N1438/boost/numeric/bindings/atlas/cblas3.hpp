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

#ifndef BOOST_NUMERIC_CBLAS_LEVEL_3_HPP
#define BOOST_NUMERIC_CBLAS_LEVEL_3_HPP

#include <cassert>

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/atlas/cblas3_overloads.hpp>
#include <boost/numeric/bindings/atlas/cblas_enum.hpp>

namespace boost { namespace numeric { namespace bindings { 

  namespace atlas {

    // C <- alpha * op (A) * op (B) + beta * C 
    // op (A) == A || A^T || A^H
    template <typename T, typename MatrA, typename MatrB, typename MatrC>
    inline
    void gemm (CBLAS_TRANSPOSE const TransA, CBLAS_TRANSPOSE const TransB, 
               T const alpha, MatrA const& a, MatrB const& b, 
               T const beta, MatrC& c
               )
    {
      typedef traits::matrix_traits<MatrA const> matraits; 
      typedef traits::matrix_traits<MatrB const> mbtraits; 
      typedef traits::matrix_traits<MatrC> mctraits; 

      int const m = TransA == CblasNoTrans
        ? matraits::size1 (a)
        : matraits::size2 (a);
      int const n = TransB == CblasNoTrans
        ? mbtraits::size2 (b)
        : mbtraits::size1 (b);
      int const k = TransA == CblasNoTrans
        ? matraits::size2 (a)
        : matraits::size1 (a); 
#ifndef NDEBUG
      int const k1 = TransB == CblasNoTrans
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
    template <typename T, typename MatrA, typename MatrB, typename MatrC>
    inline
    void gemm (T const alpha, MatrA const& a, MatrB const& b, 
               T const beta, MatrC& c) {
      gemm (CblasNoTrans, CblasNoTrans, alpha, a, b, beta, c) ;
    }
    

    // C <- A * B 
    template <typename MatrA, typename MatrB, typename MatrC>
    inline
    void gemm (MatrA const& a, MatrB const& b, MatrC& c)
    {
      typedef typename traits::matrix_traits<MatrC>::value_type val_t; 
      gemm (CblasNoTrans, CblasNoTrans, (val_t) 1, a, b, (val_t) 0, c);
    }

  } // namespace atlas

}}} 

#endif // BOOST_NUMERIC_CBLAS_LEVEL_3_HPP
