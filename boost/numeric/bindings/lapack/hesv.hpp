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

#include <complex>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/lapack/lapack.h>
//#include <boost/numeric/bindings/traits/std_vector.hpp>
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
      void hesv (char const uplo, int const n, int const nrhs,
                 std::complex<float>* a, int const lda, int* ipiv,  
                 std::complex<float>* b, int const ldb, 
                 std::complex<float>* w, int const lw, int* info) 
      {
        LAPACK_CHESV (&uplo, &n, &nrhs, 
                      reinterpret_cast<fcomplex_t*> (a), &lda, ipiv, 
                      reinterpret_cast<fcomplex_t*> (b), &ldb, 
                      reinterpret_cast<fcomplex_t*> (w), &lw, info);
      }

      inline 
      void hesv (char const uplo, int const n, int const nrhs,
                 std::complex<double>* a, int const lda, int* ipiv, 
                 std::complex<double>* b, int const ldb, 
                 std::complex<double>* w, int const lw, int* info) 
      {
        LAPACK_ZHESV (&uplo, &n, &nrhs, 
                      reinterpret_cast<dcomplex_t*> (a), &lda, ipiv, 
                      reinterpret_cast<dcomplex_t*> (b), &ldb, 
                      reinterpret_cast<dcomplex_t*> (w), &lw, info);
      }

      template <typename HermA, typename MatrB, typename IVec, typename Work>
      inline
      int hesv (char const ul, HermA& a, IVec& i, MatrB& b, 
                Work& w, int lw) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::matrix_size1 (b)); 
        assert (n == traits::vector_size (i)); 

        int info; 
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

    template <typename Herm, typename MatrB, typename IVec, typename Work>
    inline
    int hesv (char const ul, Herm& a, IVec& i, MatrB& b, Work& w) {

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<Herm>::matrix_structure, 
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

    template <typename Herm, typename MatrB, typename IVec, typename Work>
    inline
    int hesv (Herm& a, IVec& i, MatrB& b, Work& w) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<Herm>::matrix_structure, 
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

    template <typename Herm, typename MatrB>
    inline
    int hesv (char const ul, Herm& a, MatrB& b) {
      // with `internal' pivot and work vectors 

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<Herm>::matrix_structure, 
        traits::general_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      // std::vector<int> i (traits::matrix_size1 (a)); 
      int info = -101; 
      traits::detail::array<int> i (traits::matrix_size1 (a)); 

      if (i.valid()) {
        info = -102; 
        // VC 6 ?? 
        typedef typename traits::matrix_traits<Herm>::value_type val_t; 
        // std::vector<val_t> w (1); 
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

    template <typename Herm, typename MatrB>
    inline
    int hesv (Herm& a, MatrB& b) {
      // with `internal' pivot and work vectors 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<Herm>::matrix_structure, 
        traits::hermitian_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);
      // std::vector<int> i (traits::matrix_size1 (a)); 
      int info = -101; 
      traits::detail::array<int> i (traits::matrix_size1 (a)); 

      if (i.valid()) {
        info = -102; 
        // VC 6 ?? 
        typedef typename traits::matrix_traits<Herm>::value_type val_t; 
        // std::vector<val_t> w (1); 
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
      void hetrf (char const uplo, int const n, 
                  std::complex<float>* a, int const lda, int* ipiv,  
                  std::complex<float>* b, int const ldb, 
                  std::complex<float>* w, int const lw, int* info) 
      {
        LAPACK_CHETRF (&uplo, &n, 
                       reinterpret_cast<fcomplex_t*> (a), &lda, ipiv, 
                       reinterpret_cast<fcomplex_t*> (w), &lw, info);
      }

      inline 
      void hetrf (char const uplo, int const n, 
                  std::complex<double>* a, int const lda, int* ipiv, 
                  std::complex<double>* w, int const lw, int* info) 
      {
        LAPACK_ZHETRF (&uplo, &n, 
                       reinterpret_cast<dcomplex_t*> (a), &lda, ipiv, 
                       reinterpret_cast<dcomplex_t*> (w), &lw, info);
      }

      template <typename HermA, typename IVec, typename Work>
      inline
      int hetrf (char const ul, HermA& a, IVec& i, Work& w, int const lw) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::vector_size (i)); 

        int info; 
        hetrf (ul, n, traits::matrix_storage (a), 
               traits::leading_dimension (a),
               traits::vector_storage (i),  
               traits::vector_storage (w), lw, &info);
        return info; 
      }

    }

    template <typename HermA, typename IVec, typename Work>
    inline
    int hetrf (char const ul, HermA& a, IVec& i, Work& w) {

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
    int hetrf (HermA& a, IVec& i, Work& w) {

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
    int hetrf (char const ul, HermA& a, Ivec& i) {
      // with `internal' work vector

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      int info = -101; 
      // VC 6 ?? 
      typedef typename traits::matrix_traits<HermA>::value_type val_t; 
      // std::vector<val_t> w (1); 
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
    int hetrf (HermA& a, Ivec& i) {
      // with `internal' work vector 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::hermitian_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);

      int info = -101; 
      // VC 6 ?? 
      typedef typename traits::matrix_traits<HermA>::value_type val_t; 
      // std::vector<val_t> w (1); 
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
    int hetrf_query (char const ul, HermA& a) {

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      // std::vector<int> i (1); 
      traits::detail::array<int> i (1); 
      // VC 6 ?? 
      typedef typename traits::matrix_traits<HermA>::value_type val_t; 
      // std::vector<val_t> w (1); 
      traits::detail::array<val_t> w (1); 
      detail::hetrf (ul, a, i, w, -1); 
      return traits::detail::to_int (w[0]); 
    }

    template <typename HermA> 
    inline
    int hetrf_query (HermA& a) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<HermA>::matrix_structure, 
        traits::hermitian_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);
      // std::vector<int> i (1); 
      traits::detail::array<int> i (1); 
      // VC 6 ?? 
      typedef typename traits::matrix_traits<HermA>::value_type val_t; 
      // std::vector<val_t> w (1); 
      traits::detail::array<val_t> w (1); 
      detail::hetrf (uplo, a, i, w, -1); 
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
      void hetrs (char const uplo, int const n, int const nrhs,
                  std::complex<float>* a, int const lda, int* ipiv,  
                  std::complex<float>* b, int const ldb, int* info) 
      {
        LAPACK_CHETRS (&uplo, &n, &nrhs, 
                      reinterpret_cast<fcomplex_t*> (a), &lda, ipiv, 
                      reinterpret_cast<fcomplex_t*> (b), &ldb, info);
      }

      inline 
      void hetrs (char const uplo, int const n, int const nrhs,
                  std::complex<double>* a, int const lda, int* ipiv, 
                  std::complex<double>* b, int const ldb, int* info) 
      {
        LAPACK_ZHETRS (&uplo, &n, &nrhs, 
                       reinterpret_cast<dcomplex_t*> (a), &lda, ipiv, 
                       reinterpret_cast<dcomplex_t*> (b), &ldb, info);
      }

      template <typename HermA, typename MatrB, typename IVec>
      inline
      int hetrs (char const ul, HermA& a, IVec& i, MatrB& b) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::matrix_size1 (b)); 
        assert (n == traits::vector_size (i)); 

        int info; 
        hetrs (ul, n, traits::matrix_size2 (b), 
               traits::matrix_storage (a), 
               traits::leading_dimension (a),
               traits::vector_storage (i),  
               traits::matrix_storage (b),
               traits::leading_dimension (b), &info);
        return info; 
      }

    }

    template <typename HermA, typename MatrB, typename IVec>
    inline
    int hetrs (char const ul, HermA& a, IVec& i, MatrB& b) {

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
    int hetrs (HermA& a, IVec& i, MatrB& b) {

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
