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

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_GESV_HPP
#define BOOST_NUMERIC_BINDINGS_LAPACK_GESV_HPP

#include <boost/numeric/bindings/traits/type_traits.hpp>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/lapack/lapack_impl.hpp>
#include <boost/numeric/bindings/traits/detail/array.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
#  include <boost/static_assert.hpp>
#  include <boost/type_traits.hpp>
#endif 


namespace boost { namespace numeric { namespace bindings { 

  namespace lapack {

    ///////////////////////////////////////////////////////////////////
    //
    // general system of linear equations A * X = B
    // 
    ///////////////////////////////////////////////////////////////////

    /* 
     * gesv() computes the solution to a system of linear equations 
     * A * X = B, where A is an N-by-N matrix and X and B are N-by-NRHS 
     * matrices.
     *
     * The LU decomposition with partial pivoting and row interchanges 
     * is used to factor A as A = P * L * U, where P is a permutation 
     * matrix, L is unit lower triangular, and U is upper triangular.   
     * The factored form of A is then used to solve the system of 
     * equations A * X = B.
     */ 

    namespace detail {

      inline 
      void gesv (int_t const n, int_t const nrhs,
                 float* a, int_t const lda, int_t* ipiv, 
                 float* b, int_t const ldb, int_t* info) 
      {
        LAPACK_SGESV (mrc (&n), mrc (&nrhs), a, mrc (&lda), 
                      ipiv, b, mrc (&ldb), info);
      }

      inline 
      void gesv (int_t const n, int_t const nrhs,
                 double* a, int_t const lda, int_t* ipiv, 
                 double* b, int_t const ldb, int_t* info) 
      {
        LAPACK_DGESV (mrc (&n), mrc (&nrhs), a, mrc (&lda), 
                      ipiv, b, mrc (&ldb), info);
      }

      inline 
      void gesv (int_t const n, int_t const nrhs,
                 traits::complex_f* a, int_t const lda, int_t* ipiv, 
                 traits::complex_f* b, int_t const ldb, int_t* info) 
      {
        LAPACK_CGESV (mrc (&n), mrc (&nrhs), 
                      traits::complex_ptr (a), mrc (&lda), ipiv, 
                      traits::complex_ptr (b), mrc (&ldb), info);
      }
      
      inline 
      void gesv (int_t const n, int_t const nrhs,
                 traits::complex_d* a, int_t const lda, int_t* ipiv, 
                 traits::complex_d* b, int_t const ldb, int_t* info) 
      {
        LAPACK_ZGESV (mrc (&n), mrc (&nrhs), 
                      traits::complex_ptr (a), mrc (&lda), ipiv, 
                      traits::complex_ptr (b), mrc (&ldb), info);
      }

    } 

    template <typename MatrA, typename MatrB, typename IVec>
    inline
    int_t gesv (MatrA& a, IVec& ipiv, MatrB& b) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrA>::matrix_structure, 
        traits::general_t
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value)); 
#endif 

      int const n = traits::matrix_size1 (a);
      assert (n == traits::matrix_size2 (a)); 
      assert (n == traits::matrix_size1 (b)); 
      assert (n == traits::vector_size (ipiv)); 

      int_t info; 
      detail::gesv (n, traits::matrix_size2 (b), 
                    traits::matrix_storage (a), 
                    traits::leading_dimension (a),
                    traits::vector_storage (ipiv),  
                    traits::matrix_storage (b),
                    traits::leading_dimension (b),
                    &info);
      return info; 
    }

    template <typename MatrA, typename MatrB>
    inline
    int_t gesv (MatrA& a, MatrB& b) {
      // with `internal' pivot vector 
      
      // gesv() errors: 
      //   if (info == 0), successful
      //   if (info < 0), the -info argument had an illegal value
      //   -- we will use -101 if allocation fails
      //   if (info > 0), U(i-1,i-1) is exactly zero 
      int_t info = -101; 
      traits::detail::array<int_t> ipiv (traits::matrix_size1 (a)); 
      if (ipiv.valid()) 
        info = gesv (a, ipiv, b); 
      return info; 
    }


    /* 
     * getrf() computes an LU factorization of a general M-by-N matrix A  
     * using partial pivoting with row interchanges. The factorization 
     * has the form A = P * L * U, where P is a permutation matrix, 
     * L is lower triangular with unit diagonal elements (lower
     * trapezoidal if M > N), and U is upper triangular (upper 
     * trapezoidal if M < N).
     */ 

    namespace detail {

      inline 
      void getrf (int_t const n, int_t const m,
                  float* a, int_t const lda, int_t* ipiv, int_t* info) 
      {
        LAPACK_SGETRF (mrc (&n), mrc (&m), a, mrc (&lda), ipiv, info);
      }

      inline 
      void getrf (int_t const n, int_t const m,
                  double* a, int_t const lda, int_t* ipiv, int_t* info) 
      {
        LAPACK_DGETRF (mrc (&n), mrc (&m), a, mrc (&lda), ipiv, info);
      }

      inline 
      void getrf (int_t const n, int_t const m,
                  traits::complex_f* a, int_t const 
                  lda, int_t* ipiv, int_t* info) 
      {
        LAPACK_CGETRF (mrc (&n), mrc (&m), 
                       traits::complex_ptr (a), mrc (&lda), ipiv, info);
      }

      inline 
      void getrf (int_t const n, int_t const m,
                  traits::complex_d* a, int_t const lda, 
                  int_t* ipiv, int_t* info) 
      {
        LAPACK_ZGETRF (mrc (&n), mrc (&m), 
                       traits::complex_ptr (a), mrc (&lda), ipiv, info);
      }

    }

    template <typename MatrA, typename IVec>
    inline
    int_t getrf (MatrA& a, IVec& ipiv) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrA>::matrix_structure, 
        traits::general_t
      >::value)); 
#endif 

      int const n = traits::matrix_size1 (a);
      int const m = traits::matrix_size2 (a); 
      assert (traits::vector_size (ipiv) == (m < n ? m : n));

      int_t info; 
      detail::getrf (n, m, 
                     traits::matrix_storage (a), 
                     traits::leading_dimension (a),
                     traits::vector_storage (ipiv),  
                     &info);
      return info; 
    }


    /*
     * getrs() solves a system of linear equations A * X = B 
     * or A^T * X = B with a general N-by-N matrix A using  
     * the LU factorization computed by getrf().
     */

    namespace detail {

      inline 
      void getrs (char const trans, int_t const n, int_t const nrhs,
                  float const* a, int_t const lda, int_t const* ipiv, 
                  float* b, int_t const ldb, int_t* info) 
      {
        LAPACK_SGETRS (mrc (&trans), mrc (&n), mrc (&nrhs), 
                       mrc (a), mrc (&lda), mrc (ipiv), 
                       b, mrc (&ldb), info);
      }

      inline 
      void getrs (char const trans, int_t const n, int_t const nrhs,
                  double const* a, int_t const lda, int_t const* ipiv, 
                  double* b, int_t const ldb, int_t* info) 
      {
        LAPACK_DGETRS (mrc (&trans), mrc (&n), mrc (&nrhs), 
                       mrc (a), mrc (&lda), mrc (ipiv), 
                       b, mrc (&ldb), info);
      }

      inline 
      void getrs (char const trans, int_t const n, int_t const nrhs,
                  traits::complex_f const* a, int_t const lda, 
                  int_t const* ipiv, 
                  traits::complex_f* b, int_t const ldb, int_t* info) 
      {
        LAPACK_CGETRS (mrc (&trans), mrc (&n), mrc (&nrhs), 
                       traits::complex_ptr (mrc (a)), mrc (&lda), mrc (ipiv), 
                       traits::complex_ptr (b), mrc (&ldb), info);
      }

      inline 
      void getrs (char const trans, int_t const n, int_t const nrhs,
                  traits::complex_d const* a, int_t const lda, 
                  int_t const* ipiv, 
                  traits::complex_d* b, int_t const ldb, int_t* info) 
      {
        LAPACK_ZGETRS (mrc (&trans), mrc (&n), mrc (&nrhs), 
                       traits::complex_ptr (mrc (a)), mrc (&lda), mrc (ipiv), 
                       traits::complex_ptr (b), mrc (&ldb), info);
      }

    }

    template <typename MatrA, typename MatrB, typename IVec>
    inline
    int_t getrs (char const trans, MatrA const& a, IVec const& ipiv, MatrB& b) 
    {
      assert (trans == 'N' || trans == 'T' || trans == 'C'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrA>::matrix_structure, 
        traits::general_t
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value)); 
#endif 

      int const n = traits::matrix_size1 (a);
      assert (n == traits::matrix_size2 (a)); 
      assert (n == traits::matrix_size1 (b)); 
      assert (n == traits::vector_size (ipiv)); 

      int_t info; 
      detail::getrs (trans, n, traits::matrix_size2 (b), 
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
                     traits::matrix_storage (a), 
#else
                     traits::matrix_storage_const (a), 
#endif 
                     traits::leading_dimension (a),
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
                     traits::vector_storage (ipiv),  
#else
                     traits::vector_storage_const (ipiv),  
#endif
                     traits::matrix_storage (b),
                     traits::leading_dimension (b),
                     &info);
      return info; 
    }

    template <typename MatrA, typename MatrB, typename IVec>
    inline
    int_t getrs (MatrA const& a, IVec const& ipiv, MatrB& b) {
      char const no_transpose = 'N'; 
      return getrs (no_transpose, a, ipiv, b); 
    }

    // TO DO: getri() 

  }

}}}

#endif 
