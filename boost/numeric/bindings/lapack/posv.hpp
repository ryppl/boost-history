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

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_POSV_HPP
#define BOOST_NUMERIC_BINDINGS_LAPACK_POSV_HPP

#include <boost/numeric/bindings/traits/type_traits.hpp>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/lapack/lapack_impl.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
#  include <boost/numeric/bindings/traits/detail/symm_herm_traits.hpp>
#  include <boost/static_assert.hpp>
#  include <boost/type_traits.hpp>
#endif 


namespace boost { namespace numeric { namespace bindings { 

  namespace lapack {

    /////////////////////////////////////////////////////////////////////
    //
    // system of linear equations A * X = B
    // with A symmetric or Hermitian positive definite matrix
    //
    /////////////////////////////////////////////////////////////////////

    /*
     * posv() computes the solution to a system of linear equations 
     * A * X = B, where A is an N-by-N symmetric or Hermitian positive 
     * definite matrix and X and B are N-by-NRHS matrices.
     *
     * The Cholesky decomposition is used to factor A as
     *   A = U^T * U or A = U^H * U,  if UPLO = 'U', 
     *   A = L * L^T or A = L * L^H,  if UPLO = 'L',
     * where U is an upper triangular matrix and L is a lower triangular
     * matrix. The factored form of A is then used to solve the system of
     * equations A * X = B.
     * 
     * If UPLO = 'U', the leading N-by-N upper triangular part of A 
     * contains the upper triangular part of the matrix A, and the 
     * strictly lower triangular part of A is not referenced. 
     * If UPLO = 'L', the leading N-by-N lower triangular part of A 
     * contains the lower triangular part of the matrix A, and the 
     * strictly upper triangular part of A is not referenced.
     */

    namespace detail {

      inline 
      void posv (char const uplo, int_t const n, int_t const nrhs,
                 float* a, int_t const lda, 
                 float* b, int_t const ldb, int_t* info) 
      {
        LAPACK_SPOSV (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                      a, mrc (&lda), b, mrc (&ldb), info);
      }

      inline 
      void posv (char const uplo, int_t const n, int_t const nrhs,
                 double* a, int_t const lda, 
                 double* b, int_t const ldb, int_t* info) 
      {
        LAPACK_DPOSV (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                      a, mrc (&lda), b, mrc (&ldb), info);
      }

      inline 
      void posv (char const uplo, int_t const n, int_t const nrhs,
                 traits::complex_f* a, int_t const lda, 
                 traits::complex_f* b, int_t const ldb, int_t* info) 
      {
        LAPACK_CPOSV (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                      traits::complex_ptr (a), mrc (&lda), 
                      traits::complex_ptr (b), mrc (&ldb), info);
      }

      inline 
      void posv (char const uplo, int_t const n, int_t const nrhs,
                 traits::complex_d* a, int_t const lda, 
                 traits::complex_d* b, int_t const ldb, int_t* info) 
      {
        LAPACK_ZPOSV (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                      traits::complex_ptr (a), mrc (&lda), 
                      traits::complex_ptr (b), mrc (&ldb), info);
      }

      template <typename SymmMatrA, typename MatrB>
      inline
      int_t posv (char const uplo, SymmMatrA& a, MatrB& b) {
        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a));
        assert (n == traits::matrix_size1 (b));
        int_t info; 
        posv (uplo, n, traits::matrix_size2 (b),
              traits::matrix_storage (a), 
              traits::leading_dimension (a),
              traits::matrix_storage (b), 
              traits::leading_dimension (b), 
              &info);
        return info; 
      }

    }

    template <typename SymmMatrA, typename MatrB>
    inline
    int_t posv (char const uplo, SymmMatrA& a, MatrB& b) {

      assert (uplo == 'U' || uplo == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmMatrA>::matrix_structure, 
        traits::general_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      return detail::posv (uplo, a, b); 
    }

    template <typename SymmMatrA, typename MatrB>
    inline
    int_t posv (SymmMatrA& a, MatrB& b) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      typedef traits::matrix_traits<SymmMatrA> matraits;
      typedef typename matraits::value_type val_t;
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::matrix_structure,
        typename traits::detail::symm_herm_t<val_t>::type
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);
      return detail::posv (uplo, a, b); 
    }


    /*
     * potrf() computes the Cholesky factorization of a symmetric
     * or Hermitian positive definite matrix A. The factorization has 
     * the form
     *   A = U^T * U or A = U^H * U,  if UPLO = 'U', 
     *   A = L * L^T or A = L * L^H,  if UPLO = 'L',
     * where U is an upper triangular matrix and L is lower triangular.
     */

    namespace detail {

      inline 
      void potrf (char const uplo, int_t const n, 
                  float* a, int_t const lda, int_t* info) 
      {
        LAPACK_SPOTRF (mrc (&uplo), mrc (&n), a, mrc (&lda), info);
      }

      inline 
      void potrf (char const uplo, int_t const n, 
                  double* a, int_t const lda, int_t* info) 
      {
        LAPACK_DPOTRF (mrc (&uplo), mrc (&n), a, mrc (&lda), info);
      }

      inline 
      void potrf (char const uplo, int_t const n, 
                  traits::complex_f* a, int_t const lda, int_t* info) 
      {
        LAPACK_CPOTRF (mrc (&uplo), mrc (&n), 
                       traits::complex_ptr (a), mrc (&lda), info);
      }

      inline 
      void potrf (char const uplo, int_t const n, 
                  traits::complex_d* a, int_t const lda, int_t* info) 
      {
        LAPACK_ZPOTRF (mrc (&uplo), mrc (&n), 
                       traits::complex_ptr (a), mrc (&lda), info);
      }

      template <typename SymmMatrA> 
      inline
      int_t potrf (char const uplo, SymmMatrA& a) {
        int_t const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a));
        int_t info; 
        potrf (uplo, n, traits::matrix_storage (a), 
               traits::leading_dimension (a), &info);
        return info; 
      }

    }

    template <typename SymmMatrA> 
    inline
    int_t potrf (char const uplo, SymmMatrA& a) {

      assert (uplo == 'U' || uplo == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmMatrA>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      return detail::potrf (uplo, a); 
    }

    template <typename SymmMatrA>
    inline
    int_t potrf (SymmMatrA& a) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      typedef traits::matrix_traits<SymmMatrA> matraits;
      typedef typename matraits::value_type val_t;
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::matrix_structure,
        typename traits::detail::symm_herm_t<val_t>::type
      >::value));
#endif
      
      char uplo = traits::matrix_uplo_tag (a);
      return detail::potrf (uplo, a); 
    }


    /*
     * potrs() solves a system of linear equations A*X = B with 
     * a symmetric or Hermitian positive definite matrix A using 
     * the Cholesky factorization computed by potrf().
     */

    namespace detail {

      inline 
      void potrs (char const uplo, int_t const n, int_t const nrhs,
                  float const* a, int_t const lda, 
                  float* b, int_t const ldb, int_t* info) 
      {
        LAPACK_SPOTRS (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                       mrc (a), mrc (&lda), b, mrc (&ldb), info);
      }

      inline 
      void potrs (char const uplo, int_t const n, int_t const nrhs,
                  double const* a, int_t const lda, 
                  double* b, int_t const ldb, int_t* info) 
      {
        LAPACK_DPOTRS (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                       mrc (a), mrc (&lda), b, mrc (&ldb), info);
      }

      inline 
      void potrs (char const uplo, int_t const n, int_t const nrhs,
                  traits::complex_f const* a, int_t const lda, 
                  traits::complex_f* b, int_t const ldb, int_t* info) 
      {
        LAPACK_CPOTRS (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                       traits::complex_ptr (mrc (a)), mrc (&lda), 
                       traits::complex_ptr (b), mrc (&ldb), info);
      }

      inline 
      void potrs (char const uplo, int_t const n, int_t const nrhs,
                  traits::complex_d const* a, int_t const lda, 
                  traits::complex_d* b, int_t const ldb, int_t* info) 
      {
        LAPACK_ZPOTRS (mrc (&uplo), mrc (&n), mrc (&nrhs), 
                       traits::complex_ptr (mrc (a)), mrc (&lda), 
                       traits::complex_ptr (b), mrc (&ldb), info);
      }

      template <typename SymmMatrA, typename MatrB>
      inline
      int_t potrs (char const uplo, SymmMatrA const& a, MatrB& b) {
        int const n = traits::matrix_size1 (a);
        assert (n == traits::matrix_size2 (a));
        assert (n == traits::matrix_size1 (b));
        int_t info; 
        potrs (uplo, n, traits::matrix_size2 (b),
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
               traits::matrix_storage (a), 
#else
               traits::matrix_storage_const (a), 
#endif 
               traits::leading_dimension (a),
               traits::matrix_storage (b), 
               traits::leading_dimension (b), 
               &info);
        return info; 
      }

    }

    template <typename SymmMatrA, typename MatrB>
    inline
    int_t potrs (char const uplo, SymmMatrA const& a, MatrB& b) {

      assert (uplo == 'U' || uplo == 'L'); 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SymmMatrA>::matrix_structure, 
        traits::general_t
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrB>::matrix_structure, 
        traits::general_t
      >::value));
#endif

      return detail::potrs (uplo, a, b); 
    }

    template <typename SymmMatrA, typename MatrB>
    inline
    int_t potrs (SymmMatrA const& a, MatrB& b) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK
      typedef traits::matrix_traits<SymmMatrA> matraits;
      typedef traits::matrix_traits<MatrB> mbtraits;
      typedef typename matraits::value_type val_t;
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::matrix_structure,
        typename traits::detail::symm_herm_t<val_t>::type
      >::value));
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mbtraits::matrix_structure, traits::general_t
      >::value));
#endif

      char uplo = traits::matrix_uplo_tag (a);
      return detail::potrs (uplo, a, b); 
    }

    // TO DO: potri() 

  }

}}}

#endif 
