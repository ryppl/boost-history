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

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_SYSV_HPP
#define BOOST_NUMERIC_BINDINGS_LAPACK_SYSV_HPP

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
    // system of linear equations A * X = B with A symmetric matrix
    //
    /////////////////////////////////////////////////////////////////////

    /*
     * sysv() computes the solution to a system of linear equations 
     * A * X = B, where A is an N-by-N symmetric matrix and X and B 
     * are N-by-NRHS matrices.
     *
     * The diagonal pivoting method is used to factor A as
     *   A = U * D * U^T,  if UPLO = 'U', 
     *   A = L * D * L^T,  if UPLO = 'L',
     * where  U (or L) is a product of permutation and unit upper 
     * (lower) triangular matrices, and D is symmetric and block 
     * diagonal with 1-by-1 and 2-by-2 diagonal blocks.  
     * The factored form of A is then used to solve the system 
     * of equations A * X = B.
     */

    namespace detail {

      inline 
      void sysv (char const uplo, int const n, int const nrhs,
                 float* a, int const lda, int* ipiv, 
                 float* b, int const ldb, 
                 float* w, int const lw, int* info) 
      {
        LAPACK_SSYSV (&uplo, &n, &nrhs, a, &lda, ipiv, b, &ldb, w, &lw, info);
      }

      inline 
      void sysv (char const uplo, int const n, int const nrhs,
                 double* a, int const lda, int* ipiv, 
                 double* b, int const ldb, 
                 double* w, int const lw, int* info) 
      {
        LAPACK_DSYSV (&uplo, &n, &nrhs, a, &lda, ipiv, b, &ldb, w, &lw, info);
      }

      inline 
      void sysv (char const uplo, int const n, int const nrhs,
                 std::complex<float>* a, int const lda, int* ipiv,  
                 std::complex<float>* b, int const ldb, 
                 std::complex<float>* w, int const lw, int* info) 
      {
        LAPACK_CSYSV (&uplo, &n, &nrhs, 
                      reinterpret_cast<fcomplex_t*> (a), &lda, ipiv, 
                      reinterpret_cast<fcomplex_t*> (b), &ldb, 
                      reinterpret_cast<fcomplex_t*> (w), &lw, info);
      }

      inline 
      void sysv (char const uplo, int const n, int const nrhs,
                 std::complex<double>* a, int const lda, int* ipiv, 
                 std::complex<double>* b, int const ldb, 
                 std::complex<double>* w, int const lw, int* info) 
      {
        LAPACK_ZSYSV (&uplo, &n, &nrhs, 
                      reinterpret_cast<dcomplex_t*> (a), &lda, ipiv, 
                      reinterpret_cast<dcomplex_t*> (b), &ldb, 
                      reinterpret_cast<dcomplex_t*> (w), &lw, info);
      }

      template <typename SymmA, typename MatrB, typename IVec, typename Work>
      inline
      int sysv (char const ul, SymmA& a, IVec& i, MatrB& b, 
                Work& w, int lw) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::matrix_size1 (b)); 
        assert (n == traits::vector_size (i)); 

        int info; 
        sysv (ul, n, traits::matrix_size2 (b), 
              traits::matrix_storage (a), 
              traits::leading_dimension (a),
              traits::vector_storage (i),  
              traits::matrix_storage (b),
              traits::leading_dimension (b),
              traits::vector_storage (w), lw, &info);
        return info; 
      }

    }

    template <typename SymmA, typename MatrB, typename IVec, typename Work>
    inline
    int sysv (char const ul, SymmA& a, IVec& i, MatrB& b, Work& w) {

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::general_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      int const lw = traits::vector_size (w); 
      assert (lw >= 1); 
      return detail::sysv (ul, a, i, b, w, lw); 
    }

    template <typename SymmA, typename MatrB, typename IVec, typename Work>
    inline
    int sysv (SymmA& a, IVec& i, MatrB& b, Work& w) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::symmetric_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      int const lw = traits::vector_size (w); 
      assert (lw >= 1); 
      char uplo = traits::matrix_uplo_tag (a);
      return detail::sysv (uplo, a, i, b, w, lw); 
    }

    template <typename SymmA, typename MatrB>
    inline
    int sysv (char const ul, SymmA& a, MatrB& b) {
      // with `internal' pivot and work vectors 

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
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
        typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
        // std::vector<val_t> w (1); 
        traits::detail::array<val_t> w (1); 
        int lw = -1; 
        detail::sysv (ul, a, i, b, w, lw); 
        lw = traits::detail::to_int (w[0]); 
        w.resize (lw); 
        if (w.valid()) 
          info =  detail::sysv (ul, a, i, b, w, lw); 
      }
      return info; 
    }

    template <typename SymmA, typename MatrB>
    inline
    int sysv (SymmA& a, MatrB& b) {
      // with `internal' pivot and work vectors 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::symmetric_t
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
        typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
        // std::vector<val_t> w (1); 
        traits::detail::array<val_t> w (1); 
        int lw = -1; 
        detail::sysv (uplo, a, i, b, w, lw); 
        lw = traits::detail::to_int (w[0]); 
        w.resize (lw); 
        if (w.valid()) 
          info = detail::sysv (uplo, a, i, b, w, lw); 
      }
      return info; 
    }


    /*
     * sytrf() computes the factorization of a symmetric matrix A using
     * the  Bunch-Kaufman diagonal pivoting method. The form of the 
     * factorization is
     *    A = U * D * U^T  or  A = L * D * L^T
     * where U (or L) is a product of permutation and unit upper (lower)  
     * triangular matrices, and D is symmetric and block diagonal with 
     * 1-by-1 and 2-by-2 diagonal blocks.
     */

    namespace detail {

      inline 
      void sytrf (char const uplo, int const n, 
                  float* a, int const lda, int* ipiv, 
                  float* w, int const lw, int* info) 
      {
        LAPACK_SSYTRF (&uplo, &n, a, &lda, ipiv, w, &lw, info);
      }

      inline 
      void sytrf (char const uplo, int const n, 
                  double* a, int const lda, int* ipiv, 
                  double* w, int const lw, int* info) 
      {
        LAPACK_DSYTRF (&uplo, &n, a, &lda, ipiv, w, &lw, info);
      }

      inline 
      void sytrf (char const uplo, int const n, 
                  std::complex<float>* a, int const lda, int* ipiv,  
                  std::complex<float>* b, int const ldb, 
                  std::complex<float>* w, int const lw, int* info) 
      {
        LAPACK_CSYTRF (&uplo, &n, 
                       reinterpret_cast<fcomplex_t*> (a), &lda, ipiv, 
                       reinterpret_cast<fcomplex_t*> (w), &lw, info);
      }

      inline 
      void sytrf (char const uplo, int const n, 
                  std::complex<double>* a, int const lda, int* ipiv, 
                  std::complex<double>* w, int const lw, int* info) 
      {
        LAPACK_ZSYTRF (&uplo, &n, 
                       reinterpret_cast<dcomplex_t*> (a), &lda, ipiv, 
                       reinterpret_cast<dcomplex_t*> (w), &lw, info);
      }

      template <typename SymmA, typename IVec, typename Work>
      inline
      int sytrf (char const ul, SymmA& a, IVec& i, Work& w, int lw) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::vector_size (i)); 

        int info; 
        sytrf (ul, n, traits::matrix_storage (a), 
               traits::leading_dimension (a),
               traits::vector_storage (i),  
               traits::vector_storage (w), lw, &info);
        return info; 
      }

    }

    template <typename SymmA, typename IVec, typename Work>
    inline
    int sytrf (char const ul, SymmA& a, IVec& i, Work& w) {

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      int const lw = traits::vector_size (w); 
      assert (lw >= 1); 
      return detail::sytrf (ul, a, i, w, lw); 
    }

    template <typename SymmA, typename IVec, typename Work>
    inline
    int sytrf (SymmA& a, IVec& i, Work& w) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::symmetric_t
      >::value));
#endif

      int const lw = traits::vector_size (w); 
      assert (lw >= 1); 
      char uplo = traits::matrix_uplo_tag (a);
      return detail::sytrf (uplo, a, i, w, lw); 
    }

    template <typename SymmA, typename Ivec>
    inline
    int sytrf (char const ul, SymmA& a, Ivec& i) {
      // with `internal' work vector

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      int info = -101; 
      // VC 6 ?? 
      typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
      // std::vector<val_t> w (1); 
      traits::detail::array<val_t> w (1); 
      int lw = -1; 
      detail::sytrf (ul, a, i, w, lw); 
      lw = traits::detail::to_int (w[0]); 
      w.resize (lw); 
      if (w.valid()) 
        info =  detail::sytrf (ul, a, i, w, lw); 
      return info; 
    }

    template <typename SymmA, typename Ivec>
    inline
    int sytrf (SymmA& a, Ivec& i) {
      // with `internal' work vector 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::symmetric_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);

      int info = -101; 
      // VC 6 ?? 
      typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
      // std::vector<val_t> w (1); 
      traits::detail::array<val_t> w (1); 
      int lw = -1; 
      detail::sytrf (uplo, a, i, w, lw); 
      lw = traits::detail::to_int (w[0]); 
      w.resize (lw); 
      if (w.valid()) 
        info =  detail::sytrf (uplo, a, i, w, lw); 
      return info; 
    }

    template <typename SymmA>
    inline
    int sytrf_query (char const ul, SymmA& a) {

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      // std::vector<int> i (1); 
      traits::detail::array<int> i (1); 
      // VC 6 ?? 
      typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
      // std::vector<val_t> w (1); 
      traits::detail::array<val_t> w (1); 
      detail::sytrf (ul, a, i, w, -1); 
      return traits::detail::to_int ((w [0])); 
    }

    template <typename SymmA> 
    inline
    int sytrf_query (SymmA& a) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::symmetric_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);
      // std::vector<int> i (1); 
      traits::detail::array<int> i (1); 
      // VC 6 ?? 
      typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
      // std::vector<val_t> w (1); 
      traits::detail::array<val_t> w (1); 
      detail::sytrf (uplo, a, i, w, -1); 
      return traits::detail::to_int ((w [0])); 
    }


    /*
     * sytrs() solves a system of linear equations A*X = B with 
     * a symmetric matrix A using the factorization 
     *    A = U * D * U^T   or  A = L * D * L^T
     * computed by sytrf().
     */

    namespace detail {

      inline 
      void sytrs (char const uplo, int const n, int const nrhs,
                  float* a, int const lda, int* ipiv, 
                  float* b, int const ldb, int* info) 
      {
        LAPACK_SSYTRS (&uplo, &n, &nrhs, a, &lda, ipiv, b, &ldb, info);
      }

      inline 
      void sytrs (char const uplo, int const n, int const nrhs,
                  double* a, int const lda, int* ipiv, 
                  double* b, int const ldb, int* info) 
      {
        LAPACK_DSYTRS (&uplo, &n, &nrhs, a, &lda, ipiv, b, &ldb, info);
      }

      inline 
      void sytrs (char const uplo, int const n, int const nrhs,
                  std::complex<float>* a, int const lda, int* ipiv,  
                  std::complex<float>* b, int const ldb, int* info) 
      {
        LAPACK_CSYTRS (&uplo, &n, &nrhs, 
                      reinterpret_cast<fcomplex_t*> (a), &lda, ipiv, 
                      reinterpret_cast<fcomplex_t*> (b), &ldb, info);
      }

      inline 
      void sytrs (char const uplo, int const n, int const nrhs,
                  std::complex<double>* a, int const lda, int* ipiv, 
                  std::complex<double>* b, int const ldb, int* info) 
      {
        LAPACK_ZSYTRS (&uplo, &n, &nrhs, 
                       reinterpret_cast<dcomplex_t*> (a), &lda, ipiv, 
                       reinterpret_cast<dcomplex_t*> (b), &ldb, info);
      }

      template <typename SymmA, typename MatrB, typename IVec>
      inline
      int sytrs (char const ul, SymmA& a, IVec& i, MatrB& b) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::matrix_size1 (b)); 
        assert (n == traits::vector_size (i)); 

        int info; 
        sytrs (ul, n, traits::matrix_size2 (b), 
               traits::matrix_storage (a), 
               traits::leading_dimension (a),
               traits::vector_storage (i),  
               traits::matrix_storage (b),
               traits::leading_dimension (b), &info);
        return info; 
      }

    }

    template <typename SymmA, typename MatrB, typename IVec>
    inline
    int sytrs (char const ul, SymmA& a, IVec& i, MatrB& b) {

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::general_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      return detail::sytrs (ul, a, i, b); 
    }

    template <typename SymmA, typename MatrB, typename IVec>
    inline
    int sytrs (SymmA& a, IVec& i, MatrB& b) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::symmetric_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);
      return detail::sytrs (uplo, a, i, b); 
    }


    // TO DO: sytri 

  }

}}}

#endif 
