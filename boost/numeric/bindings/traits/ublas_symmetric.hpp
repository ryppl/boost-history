// (-*- C++ -*- header)  

/*
 * 
 * Copyright (c) Kresimir Fresl and Toon Knapen 2002 
 *
 * Permission to copy, modify, use and distribute this software 
 * for any non-commercial or commercial purpose is granted provided 
 * that this license appear on all copies of the software source code.
 *
 * Authors assume no responsibility whatsoever for its use and makes 
 * no guarantees about its quality, correctness or reliability.
 *
 * First author acknowledges the support of the Faculty of Civil 
 * Engineering, University of Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_SYMMETRIC_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_SYMMETRIC_H

#include <boost/numeric/ublas/symmetric.hpp> 
#include <boost/numeric/bindings/traits/traits.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_ordering.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_uplo.hpp>
#include <boost/numeric/bindings/traits/detail/generate_const.hpp>
#include <boost/numeric/bindings/traits/detail/array_type_traits.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // ublas::symmetric_matrix<>
  template <typename T, typename F1, typename F2, typename A, typename M>
  struct matrix_detail_traits<
    boost::numeric::ublas::symmetric_matrix<T, F1, F2, A> , M
  >
  {
  private:
#ifdef BOOST_BINDINGS_FORTRAN
    BOOST_STATIC_ASSERT((boost::is_same<
      typename F2::orientation_category, 
      boost::numeric::ublas::column_major_tag
    >::value)); 
#endif 
  public: 
    typedef symmetric_packed_t                                    matrix_structure; 
    typedef boost::numeric::ublas::symmetric_matrix<T, F1, F2, A> identifier_type;
    typedef M                                                     matrix_type;
    typedef T                                                     value_type ; 
    typedef typename detail::generate_const<M,T>::type*           pointer ; 

    static pointer storage (matrix_type& sm) {
      typedef typename detail::array_type_traits<matrix_type>::type array_type; 
      return vector_traits<array_type>::storage (sm.data()); 
    }
    static int size1 (matrix_type& sm) { return sm.size1(); } 
    static int size2 (matrix_type& sm) { return sm.size2(); }
    static int storage_size (matrix_type& sm) { 
      return (size1 (sm) + 1) * size2 (sm) / 2; 
    }

    typedef typename detail::ublas_uplo<
      typename F1::packed_category 
    >::type uplo_type; 
    typedef typename detail::ublas_ordering<
      typename F2::orientation_category
    >::type ordering_type; 
  }; // matrix_detail_traits< symmetric_matrix<T, F1, F2, A>, M >


  template <typename M, typename F1, typename MA>
  struct matrix_detail_traits<boost::numeric::ublas::symmetric_adaptor<M, F1>, MA>
  {
    typedef symmetric_t matrix_structure; 
    typedef boost::numeric::ublas::symmetric_adaptor<M, F1> identifier_type;
    typedef typename M::value_type value_type; 
    typedef value_type const* pointer; 

  private:
    typedef typename detail::generate_const<MA,M>::type                                 m_type; 
    typedef MA                                                                          matrix_type; 
    typedef typename matrix_type::matrix_closure_type                                   closure_t ; 
    typedef detail::closure_traits<typename detail::generate_const<MA,closure_t>::type> cl_traits; 

  public:
    static pointer storage (matrix_type& sm) {
      return matrix_traits<m_type>::storage (cl_traits::get (sm.data()));
    }
    static int size1 (matrix_type& sm) { return sm.size1(); } 
    static int size2 (matrix_type& sm) { return sm.size2(); }
    static int storage_size (matrix_type& sm) { 
      return size1 (sm) * size2 (sm); 
    }
    static int leading_dimension (matrix_type& sm) {
      return matrix_traits<m_type>::leading_dimension 
        (cl_traits::get (sm.data())); 
    }

    typedef typename matrix_traits<m_type>::ordering_type ordering_type; 
    typedef typename detail::ublas_uplo<
      typename F1::packed_category 
    >::type uplo_type; 
  }; 

}}}}

//#else // BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

//#include <boost/numeric/bindings/traits/symm_herm_raw.hpp> 

#endif // BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#endif // BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_SYMMETRIC_H
