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

#ifndef BOOST_NUMERIC_BINDINGS_CBLAS3_OVERLOADS_HPP
#define BOOST_NUMERIC_BINDINGS_CBLAS3_OVERLOADS_HPP

#include <complex> 
#include <boost/numeric/bindings/atlas/cblas_inc.hpp>


namespace boost { namespace numeric { namespace bindings { 

  namespace atlas { namespace detail {

    // C <- alpha * op (A) * op (B) + beta * C 
  
    inline 
    void gemm (CBLAS_ORDER const Order, 
	       CBLAS_TRANSPOSE const TransA, CBLAS_TRANSPOSE const TransB, 
	       int const M, int const N, int const K, 
	       float const alpha, float const* A, int const lda,
	       float const* B, int const ldb, 
	       float const beta, float* C, int const ldc) 
    {
      cblas_sgemm (Order, TransA, TransB, M, N, K, 
		   alpha, A, lda, 
		   B, ldb,
		   beta, C, ldc); 
    }

    inline 
    void gemm (CBLAS_ORDER const Order, 
	       CBLAS_TRANSPOSE const TransA, CBLAS_TRANSPOSE const TransB, 
	       int const M, int const N, int const K, 
	       double const alpha, double const* A, int const lda,
	       double const* B, int const ldb, 
	       double const beta, double* C, int const ldc) 
    {
      cblas_dgemm (Order, TransA, TransB, M, N, K, 
		   alpha, A, lda, 
		   B, ldb,
		   beta, C, ldc); 
    }

    inline 
    void gemm (CBLAS_ORDER const Order, 
	       CBLAS_TRANSPOSE const TransA, CBLAS_TRANSPOSE const TransB, 
	       int const M, int const N, int const K, 
	       std::complex<float> const& alpha, 
	       std::complex<float> const* A, int const lda,
	       std::complex<float> const* B, int const ldb, 
	       std::complex<float> const& beta, 
	       std::complex<float>* C, int const ldc) 
    {
      cblas_cgemm (Order, TransA, TransB, M, N, K, 
		   static_cast<void const*> (&alpha), 
		   static_cast<void const*> (A), lda, 
		   static_cast<void const*> (B), ldb,
		   static_cast<void const*> (&beta), 
		   static_cast<void*> (C), ldc); 
    }
    
    inline 
    void gemm (CBLAS_ORDER const Order, 
	       CBLAS_TRANSPOSE const TransA, CBLAS_TRANSPOSE const TransB, 
	       int const M, int const N, int const K, 
	       std::complex<double> const& alpha, 
	       std::complex<double> const* A, int const lda,
	       std::complex<double> const* B, int const ldb, 
	       std::complex<double> const& beta, 
	       std::complex<double>* C, int const ldc) 
    {
      cblas_zgemm (Order, TransA, TransB, M, N, K, 
		   static_cast<void const*> (&alpha), 
		   static_cast<void const*> (A), lda, 
		   static_cast<void const*> (B), ldb,
		   static_cast<void const*> (&beta), 
		   static_cast<void*> (C), ldc); 
    }
    

    
    // C <- alpha * A * B + beta * C 
    // C <- alpha * B * A + beta * C 
    // A = A^T

    inline 
    void symm (CBLAS_ORDER const Order, CBLAS_SIDE const Side,
               CBLAS_UPLO const Uplo, int const M, int const N, 
	       float const alpha, float const* A, int const lda,
	       float const* B, int const ldb, 
	       float const beta, float* C, int const ldc) 
    {
      cblas_ssymm (Order, Side, Uplo, M, N, 
		   alpha, A, lda, 
		   B, ldb,
		   beta, C, ldc); 
    }
  
    inline 
    void symm (CBLAS_ORDER const Order, CBLAS_SIDE const Side,
               CBLAS_UPLO const Uplo, int const M, int const N, 
	       double const alpha, double const* A, int const lda,
	       double const* B, int const ldb, 
	       double const beta, double* C, int const ldc) 
    {
      cblas_dsymm (Order, Side, Uplo, M, N, 
		   alpha, A, lda, 
		   B, ldb,
		   beta, C, ldc); 
    }
  
    inline 
    void symm (CBLAS_ORDER const Order, CBLAS_SIDE const Side,
               CBLAS_UPLO const Uplo, int const M, int const N, 
	       std::complex<float> const& alpha, 
               std::complex<float> const* A, int const lda,
	       std::complex<float> const* B, int const ldb, 
	       std::complex<float> const& beta, 
               std::complex<float>* C, int const ldc) 
    {
      cblas_csymm (Order, Side, Uplo, M, N, 
		   static_cast<void const*> (&alpha), 
                   static_cast<void const*> (A), lda, 
		   static_cast<void const*> (B), ldb,
		   static_cast<void const*> (&beta), 
                   static_cast<void*> (C), ldc); 
    }
  
    inline 
    void symm (CBLAS_ORDER const Order, CBLAS_SIDE const Side,
               CBLAS_UPLO const Uplo, int const M, int const N, 
	       std::complex<double> const& alpha, 
               std::complex<double> const* A, int const lda,
	       std::complex<double> const* B, int const ldb, 
	       std::complex<double> const& beta, 
               std::complex<double>* C, int const ldc) 
    {
      cblas_zsymm (Order, Side, Uplo, M, N, 
		   static_cast<void const*> (&alpha), 
                   static_cast<void const*> (A), lda, 
		   static_cast<void const*> (B), ldb,
		   static_cast<void const*> (&beta), 
                   static_cast<void*> (C), ldc); 
    }
  

    // C <- alpha * A * B + beta * C 
    // C <- alpha * B * A + beta * C 
    // A = A^H
  
    inline 
    void hemm (CBLAS_ORDER const Order, CBLAS_SIDE const Side,
               CBLAS_UPLO const Uplo, int const M, int const N, 
	       std::complex<float> const& alpha, 
               std::complex<float> const* A, int const lda,
	       std::complex<float> const* B, int const ldb, 
	       std::complex<float> const& beta, 
               std::complex<float>* C, int const ldc) 
    {
      cblas_chemm (Order, Side, Uplo, M, N, 
		   static_cast<void const*> (&alpha), 
                   static_cast<void const*> (A), lda, 
		   static_cast<void const*> (B), ldb,
		   static_cast<void const*> (&beta), 
                   static_cast<void*> (C), ldc); 
    }
  
    inline 
    void hemm (CBLAS_ORDER const Order, CBLAS_SIDE const Side,
               CBLAS_UPLO const Uplo, int const M, int const N, 
	       std::complex<double> const& alpha, 
               std::complex<double> const* A, int const lda,
	       std::complex<double> const* B, int const ldb, 
	       std::complex<double> const& beta, 
               std::complex<double>* C, int const ldc) 
    {
      cblas_zhemm (Order, Side, Uplo, M, N, 
		   static_cast<void const*> (&alpha), 
                   static_cast<void const*> (A), lda, 
		   static_cast<void const*> (B), ldb,
		   static_cast<void const*> (&beta), 
                   static_cast<void*> (C), ldc); 
    }
  
  }} // namepaces detail & atlas

}}} 


#endif // BOOST_NUMERIC_BINDINGS_CBLAS3_OVERLOADS_HPP
