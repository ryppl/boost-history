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

#ifndef BOOST_NUMERIC_BINDINGS_CLAPACK_HPP
#define BOOST_NUMERIC_BINDINGS_CLAPACK_HPP

#include <cassert>
#include <new>

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/traits/detail/symm_herm_traits.hpp>
#include <boost/numeric/bindings/atlas/clapack_overloads.hpp>
#include <boost/numeric/bindings/atlas/cblas_enum.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
#  include <boost/static_assert.hpp>
#  include <boost/type_traits/same_traits.hpp>
#endif


namespace boost { namespace numeric { namespace bindings { 

  namespace atlas {

    /////////////////////////////////////////////////////////////////////
    //
    // general system of linear equations A * X = B
    // 
    /////////////////////////////////////////////////////////////////////

    // gesv(): `driver' function 
    //
    // [comments from `clapack_dgesv.c':]
    /* clapack_xgesv computes the solution to a system of linear equations
     *   A * X = B,
     * where A is an N-by-N matrix and X and B are N-by-NRHS matrices.
     */
    // [but ATLAS FAQ says:]
    /* What's the deal with the RHS in the row-major factorization/solves?
     * Most users are confused by the row major factorization and related 
     * solves. The right-hand side vectors are probably the biggest source 
     * of confusion. The RHS array does not represent a matrix in the 
     * mathematical sense, it is instead a pasting together of the various 
     * RHS into one array for calling convenience. As such, RHS vectors are 
     * always stored contiguously, regardless of the row/col major that is 
     * chosen. This means that ldb/ldx is always independent of NRHS, and 
     * dependant on N, regardless of the row/col major setting. 
     */ 
    // That is, it seems that, if B is row-major, it should be NRHS-by-N, 
    // and RHS vectors should be its rows, not columns. 
    //
    // [comments from `clapack_dgesv.c':]
    /* The LU factorization used to factor A is dependent on the Order 
     * parameter, as detailed in the leading comments of clapack_dgetrf.
     * The factored form of A is then used to solve the system of equations 
     *   A * X = B.
     * A is overwritten with the appropriate LU factorization, and B [...]
     * is overwritten with the solution X on output.
     */
    // If B is row-major, solution vectors are its rows. 
    template <typename MatrA, typename MatrB, typename IVec>
    inline
    int gesv (MatrA& a, IVec& ipiv, MatrB& b) {
      typedef traits::matrix_traits<MatrA> matraits; 
      typedef traits::matrix_traits<MatrB> mbtraits; 
      typedef traits::vector_traits<IVec> vtraits;

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::matrix_structure, traits::general_t
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mbtraits::matrix_structure, traits::general_t
      >::value)); 
#endif 

      CBLAS_ORDER const stor_ord_a
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename matraits::ordering_type>::value); 
      CBLAS_ORDER const stor_ord_b
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename mbtraits::ordering_type>::value); 
      int const n = matraits::size1 (a);
      int const nrhs = stor_ord_b == CblasColMajor
        ? mbtraits::size2 (b)
        : mbtraits::size1 (b); 
      assert (n == matraits::size2 (a)); 
      assert (n == (stor_ord_b == CblasColMajor
                    ? mbtraits::size1 (b)
                    : mbtraits::size2 (b))); 
      assert (n == vtraits::size (ipiv)); 

      return detail::gesv (stor_ord_a, n, nrhs, 
                           matraits::storage (a), 
                           matraits::leading_dimension (a),
                           vtraits::storage (ipiv),  
                           mbtraits::storage (b),
                           mbtraits::leading_dimension (b));
    }

    template <typename MatrA, typename MatrB>
    inline
    int gesv (MatrA& a, MatrB& b) {
      // with `internal' pivot vector

      typedef traits::matrix_traits<MatrA> matraits; 
      typedef traits::matrix_traits<MatrB> mbtraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::matrix_structure, traits::general_t
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mbtraits::matrix_structure, traits::general_t
      >::value)); 
#endif 

      CBLAS_ORDER const stor_ord_a
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename matraits::ordering_type>::value); 
      CBLAS_ORDER const stor_ord_b
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename mbtraits::ordering_type>::value); 
      int const n = matraits::size1 (a);
      int const nrhs = stor_ord_b == CblasColMajor
        ? mbtraits::size2 (b)
        : mbtraits::size1 (b); 
      assert (n == matraits::size2 (a)); 
      assert (n == (stor_ord_b == CblasColMajor
                    ? mbtraits::size1 (b)
                    : mbtraits::size2 (b))); 

      int *ipiv = new (std::nothrow) int[n]; 
      int ierr = -9;  
      // clapack_dgesv() errors: 
      //   if (ierr == 0), successful
      //   if (ierr < 0), the -ierr argument had an illegal value
      //   -- there are 8 arguments, so we can use -9 if allocation fails
      //   if (ierr > 0), U(i-1,i-1) (or L(i-1,i-1)) is exactly zero 
 
      if (ipiv) {
        ierr = detail::gesv (stor_ord_a, n, nrhs, 
                             matraits::storage (a), 
                             matraits::leading_dimension (a),
                             ipiv,  
                             mbtraits::storage (b),
                             mbtraits::leading_dimension (b));
        delete[] ipiv; 
      }
      return ierr; 
    }

    template <typename MatrA, typename MatrB>
    inline
    int lu_solve (MatrA& a, MatrB& b) {
      return gesv (a, b); 
    }


    // getrf(): LU factorization of A
    // [comments from `clapack_dgetrf.c':]
    /* Computes one of two LU factorizations based on the setting of 
     * the Order parameter, as follows:
     * ---------------------------------------------------------------
     *                     Order == CblasColMajor
     * Column-major factorization of form
     *   A = P * L * U
     * where P is a row-permutation matrix, L is lower triangular with
     * unit diagonal elements (lower trapezoidal if M > N), and U is 
     * upper triangular (upper trapezoidal if M < N).
     * ---------------------------------------------------------------
     *                     Order == CblasRowMajor
     * Row-major factorization of form
     *   A = P * L * U
     * where P is a column-permutation matrix, L is lower triangular 
     * (lower trapezoidal if M > N), and U is upper triangular with 
     * unit diagonals (upper trapezoidal if M < N).
     */
    template <typename MatrA, typename IVec> 
    inline
    int getrf (MatrA& a, IVec& ipiv) {
      typedef traits::matrix_traits<MatrA> mtraits; 
      typedef traits::vector_traits<IVec> vtraits;
      
#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mtraits::matrix_structure, traits::general_t
      >::value)); 
#endif 

      CBLAS_ORDER const stor_ord
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename mtraits::ordering_type>::value); 
      int const m = mtraits::size1 (a);
      int const n = mtraits::size2 (a); 
      assert (vtraits::size (ipiv) == (m < n ? m : n)); 

      return detail::getrf (stor_ord, m, n, 
                            mtraits::storage (a), 
                            mtraits::leading_dimension (a),
                            vtraits::storage (ipiv)); 
    }

    template <typename MatrA, typename IVec> 
    inline
    int lu_factor (MatrA& a, IVec& ipiv) {
      return getrf (a, ipiv); 
    }


    // getrs(): solves a system of linear equations
    //          A * X = B  or  A' * X = B
    //          using the LU factorization previously computed by getrf()
    template <typename MatrA, typename MatrB, typename IVec>
    inline
    int getrs (CBLAS_TRANSPOSE const Trans, 
               MatrA const& a, IVec const& ipiv, MatrB& b) 
    {
      typedef traits::matrix_traits<MatrA const> matraits; 
      typedef traits::matrix_traits<MatrB> mbtraits; 
      typedef traits::vector_traits<IVec const> vtraits;

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::matrix_structure, traits::general_t
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mbtraits::matrix_structure, traits::general_t
      >::value)); 
#endif 

      CBLAS_ORDER const stor_ord_a
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename matraits::ordering_type>::value); 
      CBLAS_ORDER const stor_ord_b
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename mbtraits::ordering_type>::value); 
      int const n = matraits::size1 (a);
      int const nrhs = stor_ord_b == CblasColMajor
        ? mbtraits::size2 (b)
        : mbtraits::size1 (b); 
      assert (n == matraits::size2 (a)); 
      assert (n == (stor_ord_b == CblasColMajor
                    ? mbtraits::size1 (b)
                    : mbtraits::size2 (b))); 
      assert (n == vtraits::size (ipiv)); 
      
      return detail::getrs (stor_ord_a, Trans, n, nrhs, 
                            matraits::storage (a), 
                            matraits::leading_dimension (a),
                            vtraits::storage (ipiv),  
                            mbtraits::storage (b),
                            mbtraits::leading_dimension (b));
    }

    // getrs(): solves A * X = B (after getrf())
    template <typename MatrA, typename MatrB, typename IVec>
    inline
    int getrs (MatrA const& a, IVec const& ipiv, MatrB& b) {
      return getrs (CblasNoTrans, a, ipiv, b); 
    }

    template <typename MatrA, typename MatrB, typename IVec>
    inline
    int lu_substitute (MatrA const& a, IVec const& ipiv, MatrB& b) {
      return getrs (CblasNoTrans, a, ipiv, b); 
    }


    // getri(): computes the inverse of a matrix A 
    //          using the LU factorization previously computed by getrf() 
    template <typename MatrA, typename IVec> 
    inline
    int getri (MatrA& a, IVec& ipiv) {
      typedef traits::matrix_traits<MatrA> mtraits; 
      typedef traits::vector_traits<IVec> vtraits;
      
#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mtraits::matrix_structure, traits::general_t
      >::value)); 
#endif 

      CBLAS_ORDER const stor_ord
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename mtraits::ordering_type>::value); 
      int const n = mtraits::size1 (a);
      assert (n == mtraits::size2 (a)); 
      assert (vtraits::size (ipiv) == n); 

      return detail::getri (stor_ord, n, 
                            mtraits::storage (a), 
                            mtraits::leading_dimension (a),
                            vtraits::storage (ipiv)); 
    }

    template <typename MatrA, typename IVec> 
    inline
    int lu_invert (MatrA& a, IVec& ipiv) {
      return getri (a, ipiv); 
    }



    /////////////////////////////////////////////////////////////////////
    //
    // system of linear equations A * X = B
    // with A symmetric or Hermitian positive definite matrix
    //
    /////////////////////////////////////////////////////////////////////

#ifdef BINDINGS_NO_ATLAS_POTRS_BUG
    // posv(): `driver' function 
    //
    // [from `dposv.f' (slightly edited):]
    /* XPOSV computes the solution to a system of linear equations
     *    A * X = B,
     * where A is an N-by-N symmetric/Hermitian positive definite matrix 
     * and X and B are N-by-NRHS matrices. [See also comments of gesv().]
     *
     * A -- On entry, the symmetric/Hermitian matrix A.  
     * If UPLO = 'U', the leading N-by-N upper triangular part of A 
     * contains the upper triangular part of the matrix A, and the 
     * strictly lower triangular part of A is not referenced.  
     * If UPLO = 'L', the leading N-by-N lower triangular part of A 
     * contains the lower triangular part of the matrix A, and the 
     * strictly upper triangular part of A is not referenced.
     *
     * On exit, if INFO = 0, the factor U or L from the Cholesky
     * factorization A = U**T*U or A = L*L**T
     * [or A = U**H*U or A = L*L**H]. 
     *
     * B -- On entry, the right hand side matrix B.
     * On exit, if INFO = 0, the solution matrix X.
     */
    template <typename SymmMatrA, typename MatrB>
    inline
    int posv (SymmMatrA& a, MatrB& b) {
      typedef traits::matrix_traits<SymmMatrA> matraits;
      typedef traits::matrix_traits<MatrB> mbtraits; 
      typedef typename matraits::value_type val_t;

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::matrix_structure, 
        typename traits::detail::symm_herm_t<val_t>::type
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mbtraits::matrix_structure, traits::general_t
      >::value)); 
#endif 

      CBLAS_ORDER const stor_ord_a
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename matraits::ordering_type>::value); 
      CBLAS_ORDER const stor_ord_b
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename mbtraits::ordering_type>::value); 
      CBLAS_UPLO const uplo
        = static_cast<CBLAS_UPLO const>
        (uplo_triang<typename matraits::uplo_type>::value); 
      int const n = matraits::size1 (a);
      int const nrhs = stor_ord_b == CblasColMajor
        ? mbtraits::size2 (b)
        : mbtraits::size1 (b); 
      assert (n == matraits::size2 (a)); 
      assert (n == (stor_ord_b == CblasColMajor
                    ? mbtraits::size1 (b)
                    : mbtraits::size2 (b))); 

      return detail::posv (stor_ord_a, uplo, n, nrhs, 
                           matraits::storage (a), 
                           matraits::leading_dimension (a),
                           mbtraits::storage (b),
                           mbtraits::leading_dimension (b));
    }

    template <typename SymmMatrA, typename MatrB>
    inline
    int cholesky_solve (SymmMatrA& a, MatrB& b) {
      return posv (a, b); 
    }
#endif // BINDINGS_NO_ATLAS_POTRS_BUG


    // potrf(): Cholesky factorization of A 
    template <typename SymmMatrA>
    inline
    int potrf (SymmMatrA& a) {
      typedef traits::matrix_traits<SymmMatrA> mtraits;
      typedef typename mtraits::value_type val_t;

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mtraits::matrix_structure, 
        typename traits::detail::symm_herm_t<val_t>::type
      >::value)); 
#endif 

      CBLAS_ORDER const stor_ord
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename mtraits::ordering_type>::value); 
      CBLAS_UPLO const uplo
        = static_cast<CBLAS_UPLO const>
        (uplo_triang<typename mtraits::uplo_type>::value); 
      int const n = mtraits::size1 (a);
      assert (n == mtraits::size2 (a)); 

      return detail::potrf (stor_ord, uplo, n, 
                            mtraits::storage (a), 
                            mtraits::leading_dimension (a));
    }

    template <typename SymmMatrA>
    inline
    int cholesky_factor (SymmMatrA& a) {
      return potrf (a); 
    }


    // potrs(): solves a system of linear equations A * X = B
    //          using the Cholesky factorization computed by potrf()
    template <typename SymmMatrA, typename MatrB>
    inline
    int potrs (SymmMatrA const& a, MatrB& b) {
      typedef traits::matrix_traits<SymmMatrA const> matraits;
      typedef traits::matrix_traits<MatrB> mbtraits; 
      typedef typename matraits::value_type val_t;

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename matraits::matrix_structure, 
        typename traits::detail::symm_herm_t<val_t>::type
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mbtraits::matrix_structure, traits::general_t
      >::value)); 
#endif 

      CBLAS_ORDER const stor_ord_a
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename matraits::ordering_type>::value); 
      CBLAS_ORDER const stor_ord_b
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename mbtraits::ordering_type>::value); 
      CBLAS_UPLO const uplo
        = static_cast<CBLAS_UPLO const>
        (uplo_triang<typename matraits::uplo_type>::value); 
      int const n = matraits::size1 (a);
      int const nrhs = stor_ord_b == CblasColMajor
        ? mbtraits::size2 (b)
        : mbtraits::size1 (b); 
      assert (n == matraits::size2 (a)); 
      assert (n == (stor_ord_b == CblasColMajor
                    ? mbtraits::size1 (b)
                    : mbtraits::size2 (b))); 

#ifndef BINDINGS_NO_ATLAS_POTRS_BUG
      int ierr; 
      if (stor_ord_a == CblasColMajor)
        ierr = detail::potrs (stor_ord_a, uplo, n, nrhs, 
                              matraits::storage (a), 
                              matraits::leading_dimension (a),
                              mbtraits::storage (b),
                              mbtraits::leading_dimension (b));
      else // ATLAS bug with CblasRowMajor 
        ierr = detail::potrs_bug (stor_ord_a, uplo, n, nrhs, 
                                  matraits::storage (a), 
                                  matraits::leading_dimension (a),
                                  mbtraits::storage (b),
                                  mbtraits::leading_dimension (b));
      return ierr; 
#else 
      return detail::potrs (stor_ord_a, uplo, n, nrhs, 
                            matraits::storage (a), 
                            matraits::leading_dimension (a),
                            mbtraits::storage (b),
                            mbtraits::leading_dimension (b));
#endif 
    }

    template <typename SymmMatrA, typename MatrB>
    inline 
    int cholesky_substitute (SymmMatrA const& a, MatrB& b) {
      return potrs (a, b);
    }


#ifndef BINDINGS_NO_ATLAS_POTRS_BUG
    // posv(): `driver' function 
    template <typename SymmMatrA, typename MatrB>
    inline
    int posv (SymmMatrA& a, MatrB& b) {
      int ierr = potrf (a); 
      if (ierr == 0)
        ierr = potrs (a, b);
      return ierr; 
    }

    template <typename SymmMatrA, typename MatrB>
    inline
    int cholesky_solve (SymmMatrA& a, MatrB& b) {
      return posv (a, b); 
    }
#endif 


    // potri(): computes the inverse of a symmetric or Hermitian positive 
    //          definite matrix A using the Cholesky factorization 
    //          previously computed by potrf() 
    template <typename SymmMatrA>
    inline
    int potri (SymmMatrA& a) {
      typedef traits::matrix_traits<SymmMatrA> mtraits;
      typedef typename mtraits::value_type val_t;

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mtraits::matrix_structure, 
        typename traits::detail::symm_herm_t<val_t>::type
      >::value)); 
#endif 

      CBLAS_ORDER const stor_ord
        = static_cast<CBLAS_ORDER const>
        (storage_order<typename mtraits::ordering_type>::value); 
      CBLAS_UPLO const uplo
        = static_cast<CBLAS_UPLO const>
        (uplo_triang<typename mtraits::uplo_type>::value); 
      int const n = mtraits::size1 (a);
      assert (n == mtraits::size2 (a)); 

      return detail::potri (stor_ord, uplo, n, 
                            mtraits::storage (a), 
                            mtraits::leading_dimension (a));
    }

    template <typename SymmMatrA>
    inline
    int cholesky_invert (SymmMatrA& a) {
      return potri (a); 
    }


  } // namespace atlas

}}} 

#endif // BOOST_NUMERIC_BINDINGS_CLAPACK_HPP
