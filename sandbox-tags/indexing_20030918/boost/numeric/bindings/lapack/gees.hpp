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

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_GEES_HPP
#define BOOST_NUMERIC_BINDINGS_LAPACK_GEES_HPP

#include <complex>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/traits/type_traits.hpp>
#include <boost/numeric/bindings/lapack/lapack.h>
#include <boost/numeric/bindings/traits/detail/array.hpp>
// #include <boost/numeric/bindings/traits/std_vector.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
#  include <boost/static_assert.hpp>
#  include <boost/type_traits.hpp>
#endif 


namespace boost { namespace numeric { namespace bindings { 

  namespace lapack {

    ///////////////////////////////////////////////////////////////////
    //
    // Schur factorization of general matrix.
    // 
    ///////////////////////////////////////////////////////////////////

    /* 
     * gees() computes a Schur factorization of an N-by-N matrix A.
     *
     * The Schur decomposition is A = U S * herm(U)  where  U  is a
     * unitary matrix and S is upper triangular. The eigenvalues of A
     * are on the main diagonal of S. If A is real, S is in pseudo
     * upper triangular form.
     */ 

    namespace detail {
       template <typename V>
       void interlace(V* wr, V* wi, V* w_begin, V* w_end) {
          for ( ; w_begin!=w_end; ++wr, ++wi) {
             *w_begin = *wr; ++w_begin;
             *w_begin = *wi; ++w_begin;
	  }
       } // interlace()

      inline 
      void gees (char const jobvs, char const sort, logical_t* select, int const n,
                 float* a, int const lda, int& sdim, std::complex<float>* w,
                 float* vs, int const ldvs, float* work, int const lwork, bool* bwork,
                 int& info) 
      {
        float* wr = new float[n];
        float* wi = new float[n];
        LAPACK_SGEES (&jobvs, &sort, select, &n, a, &lda, &sdim, wr, wi, vs, &ldvs, work, &lwork, bwork, &info);
        interlace(wr, wi, traits::complex_ptr(w), traits::complex_ptr(w+n));
        delete [] wr;
        delete [] wi;
      }


      inline 
      void gees (char const jobvs, char const sort, logical_t* select, int const n,
                 double* a, int const lda, int& sdim, std::complex<double>* w,
                 double* vs, int const ldvs, double* work, int const lwork, bool* bwork,
                 int& info) 
      {
        double* wr = new double[n];
        double* wi = new double[n];
        LAPACK_DGEES (&jobvs, &sort, select, &n, a, &lda, &sdim, wr, wi, vs, &ldvs, work, &lwork, bwork, &info);
	interlace(wr, wi, traits::complex_ptr(w), traits::complex_ptr(w+n));
	delete [] wr;
	delete [] wi;
      }


      inline 
      void gees (char const jobvs, char const sort, logical_t* select, int const n,
                 std::complex<float>* a, int const lda, int& sdim, std::complex<float>* w,
		 std::complex<float>* vs, int const ldvs, std::complex<float>* work, int lwork, bool* bwork,
                 int& info) 
      {
        lwork -= n;
        LAPACK_CGEES (&jobvs, &sort, select, &n, traits::complex_ptr(a), &lda, &sdim,
                      traits::complex_ptr(w), traits::complex_ptr(vs), &ldvs,
                      traits::complex_ptr(work+n), &lwork, traits::complex_ptr(work), bwork, &info);
      }


      inline 
      void gees (char const jobvs, char const sort, logical_t* select, int const n,
                 std::complex<double>* a, int const lda, int& sdim, std::complex<double>* w,
                 std::complex<double>* vs, int const ldvs, std::complex<double>* work, int lwork, bool* bwork,
                 int& info) 
      {
        lwork -= n;
        LAPACK_ZGEES (&jobvs, &sort, select, &n, traits::complex_ptr(a), &lda, &sdim,
                      traits::complex_ptr(w), traits::complex_ptr(vs), &ldvs,
                      traits::complex_ptr(work+n), &lwork, traits::complex_ptr(work), bwork, &info);
      }

    } 

    // Compute Schur factorization with Schur vectors
    template <typename MatrA, typename SchVec, typename EigVal, typename Work>
    inline
    int gees (MatrA& a, EigVal& w, SchVec& vs, Work& work) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrA>::matrix_structure, 
        traits::general_t
      >::value)); 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<SchVec>::matrix_structure, 
        traits::general_t
      >::value)); 
#endif 

      int const n = traits::matrix_size1 (a);
      assert (n == traits::matrix_size2 (a)); 
      assert (n == traits::matrix_size1 (vs)); 
      assert (n == traits::matrix_size2 (vs)); 
      assert (n == traits::vector_size (w)); 
      assert (3*n <= traits::vector_size (work)); 

      logical_t* select=0;
      bool* bwork=0;

      int info, sdim; 
      detail::gees ('V', 'N', select, n,
                    traits::matrix_storage (a), 
                    traits::leading_dimension (a),
		    sdim,
                    traits::vector_storage (w),
                    traits::matrix_storage (vs),
                    traits::leading_dimension (vs),
                    traits::vector_storage (work),
                    traits::vector_size (work),
                    bwork,
                    info);
      return info; 
    }


    // Compute Schur factorization without Schur vectors
    template <typename MatrA, typename EigVal, typename Work>
    inline
    int gees (MatrA& a, EigVal& w, Work& work) {

#ifndef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
      BOOST_STATIC_ASSERT((boost::is_same<
        typename traits::matrix_traits<MatrA>::matrix_structure, 
        traits::general_t
      >::value)); 
#endif 

      logical_t* select=0;
      bool* bwork = 0;

      int const n = traits::matrix_size1 (a);
      assert (n == traits::matrix_size2 (a)); 
      assert (n == traits::vector_size (w)); 
      assert (3*n <= traits::vector_size (work)); 

      int info, sdim; 
      detail::gees ('N', 'N', select, n,
                    traits::matrix_storage (a), 
                    traits::leading_dimension (a),
		    sdim,
                    traits::vector_storage (w),
                    traits::matrix_storage (a),
                    traits::leading_dimension (a),
                    traits::vector_storage (work),
                    traits::vector_size (work),
                    bwork,
                    info);
      return info; 
    }

  }

}}}

#endif 
