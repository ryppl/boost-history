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
      void sysv (char const uplo, int_t const n, int_t const nrhs,
                 float* a, int_t const lda, int_t* ipiv, 
                 float* b, int_t const ldb, 
                 float* w, int_t const lw, int_t* info) 
      {
        LAPACK_SSYSV (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                      a, mrc (&lda), ipiv, 
                      b, mrc (&ldb), w, mrc (&lw), info);
      }

      inline 
      void sysv (char const uplo, int_t const n, int_t const nrhs,
                 double* a, int_t const lda, int_t* ipiv, 
                 double* b, int_t const ldb, 
                 double* w, int_t const lw, int_t* info) 
      {
        LAPACK_DSYSV (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                      a, mrc (&lda), ipiv, 
                      b, mrc (&ldb), w, mrc (&lw), info);
      }

      inline 
      void sysv (char const uplo, int_t const n, int_t const nrhs,
                 traits::complex_f* a, int_t const lda, int_t* ipiv,  
                 traits::complex_f* b, int_t const ldb, 
                 traits::complex_f* w, int_t const lw, int_t* info) 
      {
        LAPACK_CSYSV (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                      traits::complex_ptr (a), mrc (&lda), ipiv, 
                      traits::complex_ptr (b), mrc (&ldb), 
                      traits::complex_ptr (w), mrc (&lw), info);
      }

      inline 
      void sysv (char const uplo, int_t const n, int_t const nrhs,
                 traits::complex_d* a, int_t const lda, int_t* ipiv, 
                 traits::complex_d* b, int_t const ldb, 
                 traits::complex_d* w, int_t const lw, int_t* info) 
      {
        LAPACK_ZSYSV (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                      traits::complex_ptr (a), mrc (&lda), ipiv, 
                      traits::complex_ptr (b), mrc (&ldb), 
                      traits::complex_ptr (w), mrc (&lw), info);
      }

      template <typename SymmA, typename MatrB, typename IVec, typename Work>
      inline
      int_t sysv (char const ul, SymmA& a, IVec& i, MatrB& b, 
                Work& w, int const lw) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::matrix_size1 (b)); 
        assert (n == traits::vector_size (i)); 

        int_t info; 
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
    int_t sysv (char const ul, SymmA& a, IVec& i, MatrB& b, Work& w) {

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
    int_t sysv (SymmA& a, IVec& i, MatrB& b, Work& w) {

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
    int_t sysv (char const ul, SymmA& a, MatrB& b) {
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

      int_t info = -101; 
      traits::detail::array<int_t> i (traits::matrix_size1 (a)); 

      if (i.valid()) {
        info = -102; 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
        typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
#else 
        typedef typename SymmA::value_type val_t; 
#endif 
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
    int_t sysv (SymmA& a, MatrB& b) {
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
      int_t info = -101; 
      traits::detail::array<int_t> i (traits::matrix_size1 (a)); 

      if (i.valid()) {
        info = -102; 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
        typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
#else 
        typedef typename SymmA::value_type val_t; 
#endif 
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
      void sytrf (char const uplo, int_t const n, 
                  float* a, int_t const lda, int_t* ipiv, 
                  float* w, int_t const lw, int_t* info) 
      {
        LAPACK_SSYTRF (mrc (&uplo), mrc (&n), 
                       a, mrc (&lda), ipiv, w, mrc (&lw), info);
      }

      inline 
      void sytrf (char const uplo, int_t const n, 
                  double* a, int_t const lda, int_t* ipiv, 
                  double* w, int_t const lw, int_t* info) 
      {
        LAPACK_DSYTRF (mrc (&uplo), mrc (&n), 
                       a, mrc (&lda), ipiv, w, mrc (&lw), info);
      }

      inline 
      void sytrf (char const uplo, int_t const n, 
                  traits::complex_f* a, int_t const lda, int_t* ipiv,  
                  traits::complex_f* b, int_t const ldb, 
                  traits::complex_f* w, int_t const lw, int_t* info) 
      {
        LAPACK_CSYTRF (mrc (&uplo), mrc (&n), 
                       traits::complex_ptr (a), mrc (&lda), ipiv, 
                       traits::complex_ptr (w), mrc (&lw), info);
      }

      inline 
      void sytrf (char const uplo, int_t const n, 
                  traits::complex_d* a, int_t const lda, int_t* ipiv, 
                  traits::complex_d* w, int_t const lw, int_t* info) 
      {
        LAPACK_ZSYTRF (mrc (&uplo), mrc (&n), 
                       traits::complex_ptr (a), mrc (&lda), ipiv, 
                       traits::complex_ptr (w), mrc (&lw), info);
      }

      template <typename SymmA, typename IVec, typename Work>
      inline
      int_t sytrf (char const ul, SymmA& a, IVec& i, Work& w, int const lw) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::vector_size (i)); 

        int_t info; 
        sytrf (ul, n, traits::matrix_storage (a), 
               traits::leading_dimension (a),
               traits::vector_storage (i),  
               traits::vector_storage (w), lw, &info);
        return info; 
      }

    }

    template <typename SymmA, typename IVec, typename Work>
    inline
    int_t sytrf (char const ul, SymmA& a, IVec& i, Work& w) {

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
    int_t sytrf (SymmA& a, IVec& i, Work& w) {

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
    int_t sytrf (char const ul, SymmA& a, Ivec& i) {
      // with `internal' work vector

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      int_t info = -101; 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
      typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
#else 
      typedef typename SymmA::value_type val_t; 
#endif 
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
    int_t sytrf (SymmA& a, Ivec& i) {
      // with `internal' work vector 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::symmetric_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);

      int_t info = -101; 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
      typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
#else 
      typedef typename SymmA::value_type val_t; 
#endif 
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
    int sytrf_query (char const ul, SymmA const& a) {

      assert (ul == 'U' || ul == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      // std::vector<int> i (1); 
      traits::detail::array<int_t> i (1); 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
      typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
#else 
      typedef typename SymmA::value_type val_t; 
#endif 
      // std::vector<val_t> w (1); 
      traits::detail::array<val_t> w (1); 
      detail::sytrf (ul, const_cast<SymmA&> (a), i, w, -1); 
      return traits::detail::to_int ((w [0])); 
    }

    template <typename SymmA> 
    inline
    int sytrf_query (SymmA const& a) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmA>::matrix_structure, 
        traits::symmetric_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);
      // std::vector<int> i (1); 
      traits::detail::array<int_t> i (1); 
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
      typedef typename traits::matrix_traits<SymmA>::value_type val_t; 
#else 
      typedef typename SymmA::value_type val_t; 
#endif 
      // std::vector<val_t> w (1); 
      traits::detail::array<val_t> w (1); 
      detail::sytrf (uplo, const_cast<SymmA&> (a), i, w, -1); 
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
      void sytrs (char const uplo, int_t const n, int_t const nrhs,
                  float const* a, int_t const lda, int_t const* ipiv, 
                  float* b, int_t const ldb, int_t* info) 
      {
        LAPACK_SSYTRS (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                       mrc (a), mrc (&lda), mrc (ipiv), b, mrc (&ldb), info);
      }

      inline 
      void sytrs (char const uplo, int_t const n, int_t const nrhs,
                  double const* a, int_t const lda, int_t const* ipiv, 
                  double* b, int_t const ldb, int_t* info) 
      {
        LAPACK_DSYTRS (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                       mrc (a), mrc (&lda), mrc (ipiv), b, mrc (&ldb), info);
      }

      inline 
      void sytrs (char const uplo, int_t const n, int_t const nrhs,
                  traits::complex_f const* a, int_t const lda, 
                  int_t const* ipiv,  
                  traits::complex_f* b, int_t const ldb, int_t* info) 
      {
        LAPACK_CSYTRS (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                      traits::complex_ptr (mrc (a)), mrc (&lda), mrc (ipiv), 
                      traits::complex_ptr (b), mrc (&ldb), info);
      }

      inline 
      void sytrs (char const uplo, int_t const n, int_t const nrhs,
                  traits::complex_d const* a, int_t const lda, 
                  int_t const* ipiv, 
                  traits::complex_d* b, int_t const ldb, int_t* info) 
      {
        LAPACK_ZSYTRS (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                       traits::complex_ptr (mrc (a)), mrc (&lda), mrc (ipiv), 
                       traits::complex_ptr (b), mrc (&ldb), info);
      }

      template <typename SymmA, typename MatrB, typename IVec>
      inline
      int_t sytrs (char const ul, SymmA const& a, IVec const& i, MatrB& b) {

        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a)); 
        assert (n == traits::matrix_size1 (b)); 
        assert (n == traits::vector_size (i)); 

        int_t info; 
        sytrs (ul, n, traits::matrix_size2 (b), 
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

    template <typename SymmA, typename MatrB, typename IVec>
    inline
    int_t sytrs (char const ul, SymmA const& a, IVec const& i, MatrB& b) {

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
    int_t sytrs (SymmA const& a, IVec const& i, MatrB& b) {

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
