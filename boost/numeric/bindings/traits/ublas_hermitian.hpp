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

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_HERMITIAN_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_HERMITIAN_H

#include <boost/numeric/ublas/hermitian.hpp> 
#include <boost/numeric/bindings/traits/traits.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_ordering.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_uplo.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // ublas::hermitian_matrix<>
  template <typename T, typename F1, typename F2, typename A>
  struct matrix_traits<
    boost::numeric::ublas::hermitian_matrix<T, F1, F2, A>
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
    typedef hermitian_packed_t matrix_structure; 
    typedef boost::numeric::ublas::hermitian_matrix<T, F1, F2, A> matrix_type;
    typedef T value_type; 
    typedef T* pointer; 

    static pointer storage (matrix_type& hm) {
      typedef typename matrix_type::array_type array_type; 
      return vector_traits<array_type>::storage (hm.data()); 
    }
    static int size1 (matrix_type& hm) { return hm.size1(); } 
    static int size2 (matrix_type& hm) { return hm.size2(); }
    static int storage_size (matrix_type& hm) { 
      return (size1 (hm) + 1) * size2 (hm) / 2; 
    }

    typedef typename detail::ublas_uplo<
      typename F1::packed_category 
    >::type uplo_type; 
    typedef typename detail::ublas_ordering<
      typename F2::orientation_category
    >::type ordering_type; 
  }; 

  template <typename T, typename F1, typename F2, typename A>
  struct matrix_traits<
    boost::numeric::ublas::hermitian_matrix<T, F1, F2, A> const
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
    typedef hermitian_packed_t matrix_structure; 
    typedef boost::numeric::ublas::hermitian_matrix<T, F1, F2, A> matrix_type;
    typedef T value_type; 
    typedef T const* pointer; 

    static pointer storage (matrix_type const& hm) {
      typedef typename matrix_type::const_array_type array_type; 
      return vector_traits<array_type>::storage (hm.data()); 
    }
    static int size1 (matrix_type const& hm) { return hm.size1(); } 
    static int size2 (matrix_type const& hm) { return hm.size2(); }
    static int storage_size (matrix_type const& hm) { 
      return (size1 (hm) + 1) * size2 (hm) / 2; 
    }

    typedef typename detail::ublas_uplo<
      typename F1::packed_category 
    >::type uplo_type; 
    typedef typename detail::ublas_ordering<
      typename F2::orientation_category
    >::type ordering_type; 
  }; 


  // ublas::hermitian_adaptor<>
  template <typename M, typename F2>
  struct matrix_traits<boost::numeric::ublas::hermitian_adaptor<M, F2> >
  {
    typedef hermitian_t matrix_structure; 
    typedef boost::numeric::ublas::hermitian_adaptor<M, F2> matrix_type;
    typedef typename M::value_type value_type; 
    typedef value_type* pointer; 

  private:
    typedef typename matrix_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t> cl_traits; 

  public:
    static pointer storage (matrix_type& hm) {
      return matrix_traits<M>::storage (cl_traits::get (hm.data()));
    }
    static int size1 (matrix_type& hm) { return hm.size1(); } 
    static int size2 (matrix_type& hm) { return hm.size2(); }
    static int storage_size (matrix_type& hm) { 
      return size1 (hm) * size2 (hm); 
    }
    static int leading_dimension (matrix_type& hm) {
      return matrix_traits<M>::leading_dimension (cl_traits::get (hm.data())); 
    }

    typedef typename matrix_traits<M>::ordering_type ordering_type; 
    typedef typename detail::ublas_uplo<
      typename F2::packed_category 
    >::type uplo_type; 
  }; 

  template <typename M, typename F2>
  struct matrix_traits<boost::numeric::ublas::hermitian_adaptor<M, F2> const>
  {
    typedef hermitian_t matrix_structure; 
    typedef boost::numeric::ublas::hermitian_adaptor<M, F2> matrix_type;
    typedef typename M::value_type value_type; 
    typedef value_type const* pointer; 

  private:
    typedef matrix_type const c_matrix_type; 
    typedef typename c_matrix_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t const> cl_traits; 

  public:
    static pointer storage (matrix_type const& hm) {
      return matrix_traits<M const>::storage (cl_traits::get (hm.data()));
    }
    static int size1 (matrix_type const& hm) { return hm.size1(); } 
    static int size2 (matrix_type const& hm) { return hm.size2(); }
    static int storage_size (matrix_type const& hm) { 
      return size1 (hm) * size2 (hm); 
    }
    static int leading_dimension (matrix_type const& hm) {
      return matrix_traits<M const>::leading_dimension 
        (cl_traits::get (hm.data())); 
    }

    typedef typename matrix_traits<M const>::ordering_type ordering_type; 
    typedef typename detail::ublas_uplo<
      typename F2::packed_category 
    >::type uplo_type; 
  }; 

}}}}

//#else // BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

//#include <boost/numeric/bindings/traits/symm_herm_raw.hpp> 

#endif // BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#endif // BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_HERMITIAN_H
