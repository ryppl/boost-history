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

#ifndef BOOST_NUMERIC_CBLAS2_OVERLOADS_HPP
#define BOOST_NUMERIC_CBLAS2_OVERLOADS_HPP

#include <complex> 
#include <boost/numeric/bindings/atlas/cblas_inc.h>


namespace boost { namespace numeric { namespace bindings { 

  namespace atlas { namespace detail {

    // y <- alpha * op (A) * x + beta * y

    inline 
    void gemv (CBLAS_ORDER const Order, 
	       CBLAS_TRANSPOSE const TransA, int const M, int const N,
	       float const alpha, float const* A, int const lda,
	       float const* X, int const incX, 
	       float const beta, float* Y, int const incY) 
    {
      cblas_sgemv (Order, TransA, M, N, alpha, A, lda, 
		   X, incX,
		   beta, Y, incY); 
    }
    
    inline 
    void gemv (CBLAS_ORDER const Order, 
	       CBLAS_TRANSPOSE const TransA, int const M, int const N,
	       double const alpha, double const* A, int const lda,
	       double const* X, int const incX, 
	       double const beta, double* Y, int const incY) 
    {
      cblas_dgemv (Order, TransA, M, N, alpha, A, lda, 
		   X, incX,
		   beta, Y, incY); 
    }
    
    inline 
    void gemv (CBLAS_ORDER const Order, 
	       CBLAS_TRANSPOSE const TransA, int const M, int const N,
	       std::complex<float> const& alpha, 
	       std::complex<float> const* A, int const lda,
	       std::complex<float> const* X, int const incX, 
	       std::complex<float> const& beta, 
	       std::complex<float>* Y, int const incY) 
    {
      cblas_cgemv (Order, TransA, M, N, 
		   static_cast<void const*> (&alpha), 
		   static_cast<void const*> (A), lda, 
		   static_cast<void const*> (X), incX,
		   static_cast<void const*> (&beta), 
		   static_cast<void*> (Y), incY); 
    }
    
    inline 
    void gemv (CBLAS_ORDER const Order, 
	       CBLAS_TRANSPOSE const TransA, int const M, int const N,
	       std::complex<double> const& alpha, 
	       std::complex<double> const* A, int const lda,
	       std::complex<double> const* X, int const incX, 
	       std::complex<double> const& beta, 
	       std::complex<double>* Y, int const incY) 
    {
      cblas_zgemv (Order, TransA, M, N, 
		   static_cast<void const*> (&alpha), 
		   static_cast<void const*> (A), lda, 
		   static_cast<void const*> (X), incX,
		   static_cast<void const*> (&beta), 
		   static_cast<void*> (Y), incY); 
    }
    
  }} // namepaces detail & atlas

}}} 


#endif // BOOST_NUMERIC_CBLAS2_OVERLOADS_HPP
