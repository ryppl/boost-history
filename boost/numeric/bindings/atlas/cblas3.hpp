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

#ifndef BOOST_NUMERIC_BINDINGS_CBLAS_LEVEL_3_HPP
#define BOOST_NUMERIC_BINDINGS_CBLAS_LEVEL_3_HPP

#include <cassert>

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/atlas/cblas3_overloads.hpp>
#include <boost/numeric/bindings/atlas/cblas_enum.hpp>
#include <boost/type_traits/same_traits.hpp>
#include <boost/mpl/if.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
#  include <boost/static_assert.hpp>
#endif

namespace boost { namespace numeric { namespace bindings { 

  namespace atlas {

    // C <- alpha * op (A) * op (B) + beta * C 
    // op (A) == A || A^T || A^H
    template <typename T, typename MatrA, typename MatrB, typename MatrC>
    inline
    void gemm (CBLAS_TRANSPOSE const TransA, CBLAS_TRANSPOSE const TransB, 
               T const& alpha, MatrA const& a, MatrB const& b, 
               T const& beta, MatrC& c
               )
    {
      typedef traits::matrix_traits<MatrA const> matraits; 
      typedef traits::matrix_traits<MatrB const> mbtraits; 
      typedef traits::matrix_traits<MatrC> mctraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::matrix_structure, traits::general_t
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mbtraits::matrix_structure, traits::general_t
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mctraits::matrix_structure, traits::general_t
      >::value)); 

      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::ordering_type,
        typename mbtraits::ordering_type
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::ordering_type,
        typename mctraits::ordering_type
      >::value)); 
#endif 


      int const m = TransA == CblasNoTrans
        ? matraits::size1 (a)
        : matraits::size2 (a);
      int const n = TransB == CblasNoTrans
        ? mbtraits::size2 (b)
        : mbtraits::size1 (b);
      int const k = TransA == CblasNoTrans
        ? matraits::size2 (a)
        : matraits::size1 (a); 
      assert (m == mctraits::size1 (c)); 
      assert (n == mctraits::size2 (c)); 
#ifndef NDEBUG
      int const k1 = TransB == CblasNoTrans
        ? mbtraits::size1 (b)
        : mbtraits::size2 (b);
      assert (k == k1); 
#endif
      // .. what about AtlasConj? 

      CBLAS_ORDER const stor_ord
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename matraits::ordering_type>::value); 

      detail::gemm (stor_ord, TransA, TransB, m, n, k, alpha, 
                    matraits::storage (a), matraits::leading_dimension (a),
                    mbtraits::storage (b), mbtraits::leading_dimension (b),
                    beta, 
                    mctraits::storage (c), mctraits::leading_dimension (c)); 
    }


    // C <- alpha * A * B + beta * C 
    template <typename T, typename MatrA, typename MatrB, typename MatrC>
    inline
    void gemm (T const& alpha, MatrA const& a, MatrB const& b, 
               T const& beta, MatrC& c) 
    {
      gemm (CblasNoTrans, CblasNoTrans, alpha, a, b, beta, c) ;
    }
    

    // C <- A * B 
    template <typename MatrA, typename MatrB, typename MatrC>
    inline
    void gemm (MatrA const& a, MatrB const& b, MatrC& c) {
      typedef typename traits::matrix_traits<MatrC>::value_type val_t; 
      gemm (CblasNoTrans, CblasNoTrans, (val_t) 1, a, b, (val_t) 0, c);
    }



    // C <- alpha * A * B + beta * C 
    // C <- alpha * B * A + beta * C 
    // A = A^T

    namespace detail {

      // C <- alpha * A * B + beta * C ;  A = A^T
      struct symm_left {
        template <typename T, typename MatrA, typename MatrB, typename MatrC>
        static void f (T const& alpha, MatrA const& a, MatrB const& b, 
                       T const& beta, MatrC& c) 
        {
          typedef traits::matrix_traits<MatrA const> matraits; 
          typedef traits::matrix_traits<MatrB const> mbtraits; 
          typedef traits::matrix_traits<MatrC> mctraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
          BOOST_STATIC_ASSERT((boost::is_same<
            typename matraits::matrix_structure, traits::symmetric_t
          >::value)); 
          BOOST_STATIC_ASSERT((boost::is_same<
            typename mbtraits::matrix_structure, traits::general_t
          >::value));
#endif 

          int const m = mctraits::size1 (c);
          int const n = mctraits::size2 (c);

          assert (m == matraits::size1 (a) && m == matraits::size2 (a)); 
          assert (m == mbtraits::size1 (b) && n == mbtraits::size2 (b)); 

          CBLAS_ORDER const stor_ord
            = static_cast<CBLAS_ORDER const>
            (storage_order<typename matraits::ordering_type>::value); 

          CBLAS_UPLO const uplo
            = static_cast<CBLAS_UPLO const>
            (uplo_triang<typename matraits::uplo_type>::value); 

          symm (stor_ord, CblasLeft, uplo,  
                m, n, alpha, 
                matraits::storage (a), matraits::leading_dimension (a),
                mbtraits::storage (b), mbtraits::leading_dimension (b),
                beta, 
                mctraits::storage (c), mctraits::leading_dimension (c)); 
        }
      }; 

      // C <- alpha * A * B + beta * C ;  B = B^T
      struct symm_right {
        template <typename T, typename MatrA, typename MatrB, typename MatrC>
        static void f (T const& alpha, MatrA const& a, MatrB const& b, 
                       T const& beta, MatrC& c) 
        {
          typedef traits::matrix_traits<MatrA const> matraits; 
          typedef traits::matrix_traits<MatrB const> mbtraits; 
          typedef traits::matrix_traits<MatrC> mctraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
          BOOST_STATIC_ASSERT((boost::is_same<
            typename matraits::matrix_structure, traits::general_t
          >::value));
          BOOST_STATIC_ASSERT((boost::is_same<
            typename mbtraits::matrix_structure, traits::symmetric_t
          >::value));
#endif 

          int const m = mctraits::size1 (c);
          int const n = mctraits::size2 (c);

          assert (n == mbtraits::size1 (b) && n == mbtraits::size2 (b)); 
          assert (m == matraits::size1 (a) && n == matraits::size2 (a)); 

          CBLAS_ORDER const stor_ord
            = static_cast<CBLAS_ORDER const>
            (storage_order<typename matraits::ordering_type>::value); 

          CBLAS_UPLO const uplo
            = static_cast<CBLAS_UPLO const>
            (uplo_triang<typename mbtraits::uplo_type>::value); 

          symm (stor_ord, CblasRight, uplo,  
                m, n, alpha, 
                mbtraits::storage (b), mbtraits::leading_dimension (b),
                matraits::storage (a), matraits::leading_dimension (a),
                beta, 
                mctraits::storage (c), mctraits::leading_dimension (c)); 
        }
      }; 

    } 
    
    template <typename T, typename MatrA, typename MatrB, typename MatrC>
    inline
    void symm (T const& alpha, MatrA const& a, MatrB const& b, 
               T const& beta, MatrC& c)
    {
      typedef traits::matrix_traits<MatrA const> matraits; 
      typedef traits::matrix_traits<MatrB const> mbtraits; 
      typedef traits::matrix_traits<MatrC> mctraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mctraits::matrix_structure, traits::general_t
      >::value)); 

      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::ordering_type,
        typename mbtraits::ordering_type
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::ordering_type,
        typename mctraits::ordering_type
      >::value)); 
#endif 

      typedef typename
        boost::mpl::if_c<
          boost::is_same<
            typename matraits::matrix_structure, traits::symmetric_t
          >::value,
          detail::symm_left, 
          detail::symm_right
        >::type functor; 

      functor::f (alpha, a, b, beta, c); 
    }

    // C <- A * B  
    // C <- B * A  
    template <typename MatrA, typename MatrB, typename MatrC>
    inline
    void symm (MatrA const& a, MatrB const& b, MatrC& c) {
      typedef typename traits::matrix_traits<MatrC>::value_type val_t; 
      symm ((val_t) 1, a, b, (val_t) 0, c);
    }



    // C <- alpha * A * B + beta * C 
    // C <- alpha * B * A + beta * C 
    // A = A^H
    
    namespace detail {

      // C <- alpha * A * B + beta * C ;  A = A^H
      struct hemm_left {
        template <typename T, typename MatrA, typename MatrB, typename MatrC>
        static void f (T const& alpha, MatrA const& a, MatrB const& b, 
                       T const& beta, MatrC& c) 
        {
          typedef traits::matrix_traits<MatrA const> matraits; 
          typedef traits::matrix_traits<MatrB const> mbtraits; 
          typedef traits::matrix_traits<MatrC> mctraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
          BOOST_STATIC_ASSERT((boost::is_same<
            typename matraits::matrix_structure, traits::hermitian_t
          >::value)); 
          BOOST_STATIC_ASSERT((boost::is_same<
            typename mbtraits::matrix_structure, traits::general_t
          >::value));
#endif 

          int const m = mctraits::size1 (c);
          int const n = mctraits::size2 (c);

          assert (m == matraits::size1 (a) && m == matraits::size2 (a)); 
          assert (m == mbtraits::size1 (b) && n == mbtraits::size2 (b)); 

          CBLAS_ORDER const stor_ord
            = static_cast<CBLAS_ORDER const>
            (storage_order<typename matraits::ordering_type>::value); 

          CBLAS_UPLO const uplo
            = static_cast<CBLAS_UPLO const>
            (uplo_triang<typename matraits::uplo_type>::value); 

          hemm (stor_ord, CblasLeft, uplo,  
                m, n, alpha, 
                matraits::storage (a), matraits::leading_dimension (a),
                mbtraits::storage (b), mbtraits::leading_dimension (b),
                beta, 
                mctraits::storage (c), mctraits::leading_dimension (c)); 
        }
      }; 

      // C <- alpha * A * B + beta * C ;  B = B^H
      struct hemm_right {
        template <typename T, typename MatrA, typename MatrB, typename MatrC>
        static void f (T const& alpha, MatrA const& a, MatrB const& b, 
                       T const& beta, MatrC& c) 
        {
          typedef traits::matrix_traits<MatrA const> matraits; 
          typedef traits::matrix_traits<MatrB const> mbtraits; 
          typedef traits::matrix_traits<MatrC> mctraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
          BOOST_STATIC_ASSERT((boost::is_same<
            typename matraits::matrix_structure, traits::general_t
          >::value));
          BOOST_STATIC_ASSERT((boost::is_same<
            typename mbtraits::matrix_structure, traits::hermitian_t
          >::value));
#endif 

          int const m = mctraits::size1 (c);
          int const n = mctraits::size2 (c);

          assert (n == mbtraits::size1 (b) && n == mbtraits::size2 (b)); 
          assert (m == matraits::size1 (a) && n == matraits::size2 (a)); 

          CBLAS_ORDER const stor_ord
            = static_cast<CBLAS_ORDER const>
            (storage_order<typename matraits::ordering_type>::value); 

          CBLAS_UPLO const uplo
            = static_cast<CBLAS_UPLO const>
            (uplo_triang<typename mbtraits::uplo_type>::value); 

          hemm (stor_ord, CblasRight, uplo,  
                m, n, alpha, 
                mbtraits::storage (b), mbtraits::leading_dimension (b),
                matraits::storage (a), matraits::leading_dimension (a),
                beta, 
                mctraits::storage (c), mctraits::leading_dimension (c)); 
        }
      }; 

    } 
    
    template <typename T, typename MatrA, typename MatrB, typename MatrC>
    inline
    void hemm (T const& alpha, MatrA const& a, MatrB const& b, 
               T const& beta, MatrC& c)
    {
      typedef traits::matrix_traits<MatrA const> matraits; 
      typedef traits::matrix_traits<MatrB const> mbtraits; 
      typedef traits::matrix_traits<MatrC> mctraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mctraits::matrix_structure, traits::general_t
      >::value)); 

      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::ordering_type,
        typename mbtraits::ordering_type
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::ordering_type,
        typename mctraits::ordering_type
      >::value)); 
#endif 

      typedef typename
        boost::mpl::if_c<
          boost::is_same<
            typename matraits::matrix_structure, traits::hermitian_t
          >::value,
          detail::hemm_left, 
          detail::hemm_right
        >::type functor; 

      functor::f (alpha, a, b, beta, c); 

    }

    // C <- A * B  
    // C <- B * A  
    template <typename MatrA, typename MatrB, typename MatrC>
    inline
    void hemm (MatrA const& a, MatrB const& b, MatrC& c) {
      typedef typename traits::matrix_traits<MatrC>::value_type val_t; 
      hemm ((val_t) 1, a, b, (val_t) 0, c);
    }

  } // namespace atlas

}}} 

#endif // BOOST_NUMERIC_BINDINGS_CBLAS_LEVEL_3_HPP
