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

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_SYMMETRIC_ADAPTOR_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_SYMMETRIC_ADAPTOR_H

#include <boost/numeric/ublas/symmetric.hpp> 
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_uplo.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // ublas::symmetric_adaptor<>
  template <typename M, typename F2>
  struct symmetric_adaptor_traits<
    boost::numeric::ublas::symmetric_adaptor<M, F2>
  >
  {
    typedef boost::numeric::ublas::symmetric_adaptor<M, F2> matrix_type;
    typedef typename M::value_type value_type; 
    typedef value_type* pointer; 

  private:
    typedef typename matrix_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t> cl_traits; 

  public:
    static pointer storage (matrix_type& sm) {
      return matrix_traits<M>::storage (cl_traits::get (sm.data()));
    }
    static int size1 (matrix_type& sm) { return sm.size1(); } 
    static int size2 (matrix_type& sm) { return sm.size2(); }
    static int leading_dimension (matrix_type& sm) {
      return matrix_traits<M>::leading_dimension (cl_traits::get (sm.data())); 
    }

    typedef typename matrix_traits<M>::ordering_type ordering_type; 
    typedef typename detail::ublas_uplo<
      typename F2::packed_category 
    >::type uplo_type; 
  }; 

  template <typename M, typename F2>
  struct symmetric_adaptor_traits<
    boost::numeric::ublas::symmetric_adaptor<M, F2> const
  >
  {
    typedef boost::numeric::ublas::symmetric_adaptor<M, F2> matrix_type;
    typedef typename M::value_type value_type; 
    typedef value_type const* pointer; 

  private:
    typedef matrix_type const c_matrix_type; 
    typedef typename c_matrix_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t const> cl_traits; 

  public:
    static pointer storage (matrix_type const& sm) {
      return matrix_traits<M const>::storage (cl_traits::get (sm.data()));
    }
    static int size1 (matrix_type const& sm) { return sm.size1(); } 
    static int size2 (matrix_type const& sm) { return sm.size2(); }
    static int leading_dimension (matrix_type const& sm) {
      return matrix_traits<M const>::leading_dimension 
        (cl_traits::get (sm.data())); 
    }

    typedef typename matrix_traits<M const>::ordering_type ordering_type; 
    typedef typename detail::ublas_uplo<
      typename F2::packed_category 
    >::type uplo_type; 
  }; 

}}}}

#endif 
