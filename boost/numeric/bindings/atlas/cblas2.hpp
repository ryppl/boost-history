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

#ifndef BOOST_NUMERIC_BINDINGS_CBLAS_LEVEL_2_HPP
#define BOOST_NUMERIC_BINDINGS_CBLAS_LEVEL_2_HPP

#include <cassert>

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/atlas/cblas2_overloads.hpp>
#include <boost/numeric/bindings/atlas/cblas_enum.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
#  include <boost/static_assert.hpp>
#  include <boost/type_traits/same_traits.hpp>
#endif


namespace boost { namespace numeric { namespace bindings { 

  namespace atlas {


    // y <- alpha * op (A) * x + beta * y
    // op (A) == A || A^T || A^H

    template <typename T, typename Matr, typename VctX, typename VctY>
    inline 
    void gemv (CBLAS_TRANSPOSE const TransA, 
	       T const& alpha, Matr const& a, VctX const& x, 
	       T const& beta, VctY& y
	       )
    {
      typedef traits::matrix_traits<Matr const> mtraits; 
      typedef traits::vector_traits<VctX const> xvtraits; 
      typedef traits::vector_traits<VctY> yvtraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mtraits::matrix_structure, traits::general_t
      >::value)); 
#endif 

      int const m = mtraits::size1 (a);
      int const n = mtraits::size2 (a);
      assert (xvtraits::size (x) >= (TransA == CblasNoTrans ? n : m)); 
      assert (yvtraits::size (y) >= (TransA == CblasNoTrans ? m : n)); 
      // .. what about AtlasConj? 

      CBLAS_ORDER const stor_ord
	= static_cast<CBLAS_ORDER const>
	(storage_order<typename mtraits::ordering_type>::value); 

      detail::gemv (stor_ord, TransA, m, n, alpha, mtraits::storage (a), 
		    mtraits::leading_dimension (a),
		    xvtraits::storage (x), xvtraits::stride (x),
		    beta, yvtraits::storage (y), yvtraits::stride (y));
    }

    // y <- alpha * A * x + beta * y 
    template <typename T, typename Matr, typename VctX, typename VctY>
    inline 
    void gemv (T const& alpha, Matr const& a, VctX const& x, 
	       T const& beta, VctY& y) {
      gemv (CblasNoTrans, alpha, a, x, beta, y); 
    }

    // y <- A * x
    template <typename Matr, typename VctX, typename VctY>
    inline 
    void gemv (Matr const& a, VctX const& x, VctY& y) {
      typedef typename traits::matrix_traits<Matr>::value_type val_t; 
      gemv (CblasNoTrans, (val_t) 1, a, x, (val_t) 0, y);
    }


    // y <- alpha * A * x + beta * y
    // A real symmetric matrix (T == float | double)
    // [from `dsymv.f':]
    /* [...] with UPLO = 'U' or 'u', the  leading n by n upper 
     * triangular part of the array A must contain the upper triangular
     * part of the symmetric matrix and the strictly lower triangular 
     * part of A is not referenced.
     * [...] with UPLO = 'L' or 'l', the leading n by n lower
     * triangular part of the array A must contain the lower triangular  
     * part of the symmetric matrix and the strictly  upper
     * triangular part of A is not referenced.
     */ 
    template <typename T, typename SymmMatr, typename VctX, typename VctY>
    inline 
    void symv (T const& alpha, SymmMatr const& a, VctX const& x, 
	       T const& beta, VctY& y)
    {
      typedef traits::matrix_traits<SymmMatr const> mtraits; 
      typedef traits::vector_traits<VctX const> xvtraits; 
      typedef traits::vector_traits<VctY> yvtraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mtraits::matrix_structure, traits::symmetric_t
      >::value)); 
#endif 

      int const n = mtraits::size1 (a);
      assert (n == mtraits::size2 (a)); 
      assert (xvtraits::size (x) >= n); 
      assert (yvtraits::size (y) >= n); 

      CBLAS_ORDER const stor_ord
	= static_cast<CBLAS_ORDER const>
	(storage_order<typename mtraits::ordering_type>::value); 
      CBLAS_UPLO const uplo
        = static_cast<CBLAS_UPLO const>
        (uplo_triang<typename mtraits::uplo_type>::value); 

      detail::symv (stor_ord, uplo, n, alpha, 
                    mtraits::storage (a), mtraits::leading_dimension (a),
		    xvtraits::storage (x), xvtraits::stride (x),
		    beta, yvtraits::storage (y), yvtraits::stride (y));
    }

    // y <- A * x
    template <typename SymmMatr, typename VctX, typename VctY>
    inline 
    void symv (SymmMatr const& a, VctX const& x, VctY& y) {
      typedef typename 
        traits::matrix_traits<SymmMatr>::value_type val_t; 
      symv ((val_t) 1, a, x, (val_t) 0, y);
    }


    // y <- alpha * A * x + beta * y
    // A real symmetric matrix (T == float | double) in packed form
    // [from `dspmv.f' (description assumes column major order):]
    /* A is an array of DIMENSION ( ( n*( n + 1 ) )/2 ).
     * Before entry with UPLO = 'U' or 'u', the array A must contain
     * the upper triangular  part of the symmetric matrix packed 
     * sequentially, column by column, so that A( 1 ) contains a(1,1),
     * A( 2 ) and A( 3 ) contain a(1,2) and a(2,2) respectively, and
     * so on.
     * Before entry with UPLO = 'L' or 'l', the array A must contain
     * the lower triangular  part of the symmetric matrix packed 
     * sequentially, column by column, so that A( 1 ) contains a(1,1),
     * A( 2 ) and A( 3 ) contain a(2,1) and a(3,1) respectively, and
     * so on. 
     */ 
    template <typename T, typename SymmMatr, typename VctX, typename VctY>
    inline 
    void spmv (T const& alpha, SymmMatr const& a, VctX const& x, 
	       T const& beta, VctY& y)
    {
      typedef traits::matrix_traits<SymmMatr const> mtraits; 
      typedef traits::vector_traits<VctX const> xvtraits; 
      typedef traits::vector_traits<VctY> yvtraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mtraits::matrix_structure, traits::symmetric_packed_t
      >::value)); 
#endif 

      int const n = mtraits::size1 (a);
      assert (n == mtraits::size2 (a)); 
      assert (xvtraits::size (x) >= n); 
      assert (yvtraits::size (y) >= n); 

      CBLAS_ORDER const stor_ord
	= static_cast<CBLAS_ORDER const>
	(storage_order<typename mtraits::ordering_type>::value); 
      CBLAS_UPLO const uplo
        = static_cast<CBLAS_UPLO const>
        (uplo_triang<typename mtraits::uplo_type>::value); 

      detail::spmv (stor_ord, uplo, n, alpha, mtraits::storage (a), 
		    xvtraits::storage (x), xvtraits::stride (x),
		    beta, yvtraits::storage (y), yvtraits::stride (y));
    }

    // y <- A * x
    template <typename SymmMatr, typename VctX, typename VctY>
    inline 
    void spmv (SymmMatr const& a, VctX const& x, VctY& y) {
      typedef typename 
        traits::matrix_traits<SymmMatr>::value_type val_t; 
      spmv ((val_t) 1, a, x, (val_t) 0, y);
    }


    // y <- alpha * A * x + beta * y
    // A complex hermitian matrix 
    // (T == std::complex<float> | std::complex<double>)
    template <typename T, typename HermMatr, typename VctX, typename VctY>
    inline 
    void hemv (T const& alpha, HermMatr const& a, VctX const& x, 
	       T const& beta, VctY& y)
    {
      typedef traits::matrix_traits<HermMatr const> mtraits; 
      typedef traits::vector_traits<VctX const> xvtraits; 
      typedef traits::vector_traits<VctY> yvtraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mtraits::matrix_structure, traits::hermitian_t
      >::value)); 
#endif 

      int const n = mtraits::size1 (a);
      assert (n == mtraits::size2 (a)); 
      assert (xvtraits::size (x) >= n); 
      assert (yvtraits::size (y) >= n); 

      CBLAS_ORDER const stor_ord
	= static_cast<CBLAS_ORDER const>
	(storage_order<typename mtraits::ordering_type>::value); 
      CBLAS_UPLO const uplo
        = static_cast<CBLAS_UPLO const>
        (uplo_triang<typename mtraits::uplo_type>::value); 

      detail::hemv (stor_ord, uplo, n, alpha, 
                    mtraits::storage (a), mtraits::leading_dimension (a),
		    xvtraits::storage (x), xvtraits::stride (x),
		    beta, yvtraits::storage (y), yvtraits::stride (y));
    }

    // y <- A * x
    template <typename HermMatr, typename VctX, typename VctY>
    inline 
    void hemv (HermMatr const& a, VctX const& x, VctY& y) {
      typedef typename 
        traits::matrix_traits<HermMatr>::value_type val_t; 
      hemv ((val_t) 1, a, x, (val_t) 0, y);
    }


    // y <- alpha * A * x + beta * y
    // A complex hermitian matrix in packed form 
    // (T == std::complex<float> | std::complex<double>)
    template <typename T, typename HermMatr, typename VctX, typename VctY>
    inline 
    void hpmv (T const& alpha, HermMatr const& a, VctX const& x, 
	       T const& beta, VctY& y)
    {
      typedef traits::matrix_traits<HermMatr const> mtraits; 
      typedef traits::vector_traits<VctX const> xvtraits; 
      typedef traits::vector_traits<VctY> yvtraits; 

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename mtraits::matrix_structure, traits::hermitian_packed_t
      >::value)); 
#endif 

      int const n = mtraits::size1 (a);
      assert (n == mtraits::size2 (a)); 
      assert (xvtraits::size (x) >= n); 
      assert (yvtraits::size (y) >= n); 

      CBLAS_ORDER const stor_ord
	= static_cast<CBLAS_ORDER const>
	(storage_order<typename mtraits::ordering_type>::value); 
      CBLAS_UPLO const uplo
        = static_cast<CBLAS_UPLO const>
        (uplo_triang<typename mtraits::uplo_type>::value); 

      detail::hpmv (stor_ord, uplo, n, alpha, mtraits::storage (a), 
		    xvtraits::storage (x), xvtraits::stride (x),
		    beta, yvtraits::storage (y), yvtraits::stride (y));
    }

    // y <- A * x
    template <typename HermMatr, typename VctX, typename VctY>
    inline 
    void hpmv (HermMatr const& a, VctX const& x, VctY& y) {
      typedef typename 
        traits::matrix_traits<HermMatr>::value_type val_t; 
      hpmv ((val_t) 1, a, x, (val_t) 0, y);
    }


  } // namespace atlas

}}} 

#endif // BOOST_NUMERIC_BINDINGS_CBLAS_LEVEL_2_HPP
