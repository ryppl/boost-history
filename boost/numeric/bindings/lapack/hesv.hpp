/*
 * 
 * Copyright (c) Toon Knapen & Kresimir Fresl 2003
 *
 * Permission to copy, modify, use and distribute this software 
 * for any non-commercial or commercial purpose is granted provided 
 * that this license appear on all copies of the software source code.
 *
 * Authors assume no responsibility whatsoever for its use and makes 
 * no guarantees about its quality, correctness or reliability.
 *
 * KF acknowledges the support of the Faculty of Civil Engineering, 
 * University of Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_HESV_HPP
#define BOOST_NUMERIC_BINDINGS_LAPACK_HESV_HPP

#include <boost/numeric/bindings/traits/type_traits.hpp>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/lapack/lapack_impl.hpp>
#include <boost/numeric/bindings/traits/detail/array.hpp>
#include <boost/numeric/bindings/traits/detail/utils.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
#  include <boost/static_assert.hpp>
#  include <boost/type_traits.hpp>
#endif 


namespace boost { namespace numeric { namespace bindings { 

  namespace lapack {

    /////////////////////////////////////////////////////////////////////
    //
    // system of linear equations A * X = B
    // with A Hermitian indefinite matrix
    //
    /////////////////////////////////////////////////////////////////////

    /*
     * hesv() computes the solution to a system of linear equations 
     * A * X = B, where A is an N-by-N Hermitian matrix and X and B 
     * are N-by-NRHS matrices.
     *
     * The diagonal pivoting method is used to factor A as
     *   A = U * D * U^H,  if UPLO = 'U', 
     *   A = L * D * L^H,  if UPLO = 'L',
     * where  U (or L) is a product of permutation and unit upper 
     * (lower) triangular matrices, and D is Hermitian and block 
     * diagonal with 1-by-1 and 2-by-2 diagonal blocks. The factored 
     * form of A is then used to solve the system of equations A * X = B.
     */

    namespace detail {

      inline 
      void hesv (char const uplo, int_t const n, int_t const nrhs,
                 traits::complex_f* a, int_t const lda, int_t* ipiv,  
                 traits::complex_f* b, int_t const ldb, 
                 traits::complex_f* w, int_t const lw, int_t* info) 
      {
        LAPACK_CHESV (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                      traits::complex_ptr (a), mrc (&lda), ipiv, 
                      traits::complex_ptr (b), mrc (&ldb), 
                      traits::complex_ptr (w), mrc (&lw), info);
      }

      inline 
      void hesv (char const uplo, int_t const n, int_t const nrhs,
                 traits::complex_d* a, int_t const lda, int_t* ipiv, 
                 traits::complex_d* b, int_t const ldb, 
                 traits::complex_d* w, int_t const lw, int_t* info) 
      {
        LAPACK_ZHESV (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                      traits::complex_ptr (a), mrc (&lda), ipiv, 
                      traits::complex_ptr (b), mrc (&ldb), 
                      traits::complex_ptr (w), mrc (&lw), info);
      }

      template <typename HermA, typename MatrB, typename IVec, typename Work>
      inline
      int_t hesv (char const ul, HermA& a, IVec& i, 
                  MatrB& b, Work& w, int const lw) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::matrix_size1 (b)); 
        assert (n == traits::vector_size (i)); 

        int_t info; 
        hesv (ul, n, traits::matrix_size2 (b), 
              traits::matrix_storage (a), 
              traits::leading_dimension (a),
              traits::vector_storage (i),  
              traits::matrix_storage (b),
              traits::leading_dimension (b),
              traits::vector_storage (w), lw, &info);
        return info; 
      }

    }

    template <typename HermA, typename MatrB, typename IVec, typename Work>
    inline
    int_t hesv (char const ul, HermA& a, IVec& i, MatrB& b, Work& w) {

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::general_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      int const lw = traits::vector_size (w); 
      assert (lw >= 1); 
      return detail::hesv (ul, a, i, b, w, lw); 
    }

    template <typename HermA, typename MatrB, typename IVec, typename Work>
    inline
    int_t hesv (HermA& a, IVec& i, MatrB& b, Work& w) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::hermitian_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      int const lw = traits::vector_size (w); 
      assert (lw >= 1); 
      char uplo = traits::matrix_uplo_tag (a);
      return detail::hesv (uplo, a, i, b, w, lw); 
    }

    template <typename HermA, typename MatrB>
    inline
    int_t hesv (char const ul, HermA& a, MatrB& b) {
      // with `internal' pivot and work vectors 

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::general_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      int_t info = -101; 
      traits::detail::array<int_t> i (traits::matrix_size1 (a)); 

      if (i.valid()) {
        info = -102; 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
        typedef typename traits::matrix_traits<HermA>::value_type val_t; 
#else 
        typedef typename HermA::value_type val_t; 
#endif 
        traits::detail::array<val_t> w (1); 
        // paranoia ?
        int lw = -1; 
        detail::hesv (ul, a, i, b, w, lw); 
        lw = traits::detail::to_int (w[0]); 
        w.resize (lw);
        if (w.valid()) 
          info = detail::hesv (ul, a, i, b, w, lw); 
      }
      return info; 
    }

    template <typename HermA, typename MatrB>
    inline
    int_t hesv (HermA& a, MatrB& b) {
      // with `internal' pivot and work vectors 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::hermitian_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);
      int_t info = -101; 
      traits::detail::array<int_t> i (traits::matrix_size1 (a)); 

      if (i.valid()) {
        info = -102; 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
        typedef typename traits::matrix_traits<HermA>::value_type val_t; 
#else 
        typedef typename HermA::value_type val_t; 
#endif 
        traits::detail::array<val_t> w (1); 
        int lw = -1; 
        detail::hesv (uplo, a, i, b, w, lw); 
        lw = traits::detail::to_int (w[0]); 
        w.resize (lw); 
        if (w.valid()) 
          info = detail::hesv (uplo, a, i, b, w, lw); 
      }
      return info; 
    }


    /*
     * hetrf() computes the factorization of a Hermitian matrix A using
     * the  Bunch-Kaufman diagonal pivoting method. The form of the 
     * factorization is
     *    A = U * D * U^H  or  A = L * D * L^H
     * where U (or L) is a product of permutation and unit upper (lower)  
     * triangular matrices, and D is Hermitian and block diagonal with 
     * 1-by-1 and 2-by-2 diagonal blocks.
     */

    namespace detail {

      inline 
      void hetrf (char const uplo, int_t const n, 
                  traits::complex_f* a, int_t const lda, int_t* ipiv,  
                  traits::complex_f* w, int_t const lw, int_t* info) 
      {
        LAPACK_CHETRF (mrc (&uplo), mrc (&n), 
                       traits::complex_ptr (a), mrc (&lda), ipiv, 
                       traits::complex_ptr (w), mrc (&lw), info);
      }

      inline 
      void hetrf (char const uplo, int_t const n, 
                  traits::complex_d* a, int_t const lda, int_t* ipiv, 
                  traits::complex_d* w, int_t const lw, int_t* info) 
      {
        LAPACK_ZHETRF (mrc (&uplo), mrc (&n), 
                       traits::complex_ptr (a), mrc (&lda), ipiv, 
                       traits::complex_ptr (w), mrc (&lw), info);
      }

      template <typename HermA, typename IVec, typename Work>
      inline
      int_t hetrf (char const ul, HermA& a, IVec& i, Work& w, int const lw) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::vector_size (i)); 

        int_t info; 
        hetrf (ul, n, traits::matrix_storage (a), 
               traits::leading_dimension (a),
               traits::vector_storage (i),  
               traits::vector_storage (w), lw, &info);
        return info; 
      }

    }

    template <typename HermA, typename IVec, typename Work>
    inline
    int_t hetrf (char const ul, HermA& a, IVec& i, Work& w) {

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      int const lw = traits::vector_size (w); 
      assert (lw >= 1); 
      return detail::hetrf (ul, a, i, w, lw); 
    }

    template <typename HermA, typename IVec, typename Work>
    inline
    int_t hetrf (HermA& a, IVec& i, Work& w) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::hermitian_t
      >::value));
#endif

      int const lw = traits::vector_size (w); 
      assert (lw >= 1); 
      char uplo = traits::matrix_uplo_tag (a);
      return detail::hetrf (uplo, a, i, w, lw); 
    }

    template <typename HermA, typename Ivec>
    inline
    int_t hetrf (char const ul, HermA& a, Ivec& i) {
      // with `internal' work vector

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      int_t info = -101; 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
      typedef typename traits::matrix_traits<HermA>::value_type val_t; 
#else 
      typedef typename HermA::value_type val_t; 
#endif 
      traits::detail::array<val_t> w (1); 
      int lw = -1; 
      detail::hetrf (ul, a, i, w, lw); 
      lw = traits::detail::to_int (w[0]); 
      w.resize (lw); 
      if (w.valid()) 
        info = detail::hetrf (ul, a, i, w, lw); 
      return info; 
    }

    template <typename HermA, typename Ivec>
    inline
    int_t hetrf (HermA& a, Ivec& i) {
      // with `internal' work vector 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::hermitian_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);

      int_t info = -101; 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
      typedef typename traits::matrix_traits<HermA>::value_type val_t; 
#else 
      typedef typename HermA::value_type val_t; 
#endif 
      traits::detail::array<val_t> w (1); 
      int lw = -1; 
      detail::hetrf (uplo, a, i, w, lw); 
      lw = traits::detail::to_int (w[0]); 
      w.resize (lw); 
      if (w.valid()) 
        info = detail::hetrf (uplo, a, i, w, lw); 
      return info; 
    }

    template <typename HermA>
    inline
    int hetrf_query (char const ul, HermA const& a) {

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      traits::detail::array<int_t> i (1); 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
      typedef typename traits::matrix_traits<HermA>::value_type val_t; 
#else 
      typedef typename HermA::value_type val_t; 
#endif 
      traits::detail::array<val_t> w (1); 
      detail::hetrf (ul, const_cast<HermA&> (a), i, w, -1); 
      return traits::detail::to_int (w[0]); 
    }

    template <typename HermA> 
    inline
    int hetrf_query (HermA const& a) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::hermitian_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);
      traits::detail::array<int_t> i (1); 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
      typedef typename traits::matrix_traits<HermA>::value_type val_t; 
#else 
      typedef typename HermA::value_type val_t; 
#endif 
      traits::detail::array<val_t> w (1); 
      detail::hetrf (uplo, const_cast<HermA&> (a), i, w, -1); 
      return traits::detail::to_int (w[0]); 
    }


    /*
     * hetrs() solves a system of linear equations A*X = B with 
     * a Hermitian matrix A using the factorization 
     *    A = U * D * U^H   or  A = L * D * L^H
     * computed by hetrf().
     */

    namespace detail {

      inline 
      void hetrs (char const uplo, int_t const n, int_t const nrhs,
                  traits::complex_f const* a, int_t const lda, 
                  int_t const* ipiv,  
                  traits::complex_f* b, int_t const ldb, int_t* info) 
      {
        LAPACK_CHETRS (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                       traits::complex_ptr (mrc (a)), mrc (&lda), mrc (ipiv), 
                       traits::complex_ptr (b), mrc (&ldb), info);
      }

      inline 
      void hetrs (char const uplo, int_t const n, int_t const nrhs,
                  traits::complex_d const* a, int_t const lda, 
                  int_t const* ipiv, 
                  traits::complex_d* b, int_t const ldb, int_t* info) 
      {
        LAPACK_ZHETRS (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                       traits::complex_ptr (mrc (a)), mrc (&lda), mrc (ipiv), 
                       traits::complex_ptr (b), mrc (&ldb), info);
      }

      template <typename HermA, typename MatrB, typename IVec>
      inline
      int_t hetrs (char const ul, HermA const& a, IVec const& i, MatrB& b) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::matrix_size1 (b)); 
        assert (n == traits::vector_size (i)); 

        int_t info; 
        hetrs (ul, n, traits::matrix_size2 (b), 
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
               traits::matrix_storage (a), 
#else
               traits::matrix_storage_const (a), 
#endif 
               traits::leading_dimension (a),
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
               traits::vector_storage (i),  
#else
               traits::vector_storage_const (i),  
#endif
               traits::matrix_storage (b),
               traits::leading_dimension (b), &info);
        return info; 
      }

    }

    template <typename HermA, typename MatrB, typename IVec>
    inline
    int_t hetrs (char const ul, HermA const& a, IVec const& i, MatrB& b) {

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::general_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      return detail::hetrs (ul, a, i, b); 
    }

    template <typename HermA, typename MatrB, typename IVec>
    inline
    int_t hetrs (HermA const& a, IVec const& i, MatrB& b) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::hermitian_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);
      return detail::hetrs (uplo, a, i, b); 
    }


    // TO DO: hetri

  }

}}}

#endif 
