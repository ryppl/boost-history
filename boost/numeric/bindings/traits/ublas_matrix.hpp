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

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_MATRIX_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_MATRIX_H

#include <boost/numeric/ublas/matrix.hpp> 
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_matrix_closure.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_ordering.hpp>

#ifdef BOOST_BINDINGS_FORTRAN
#  include <boost/static_assert.hpp>
#  include <boost/type_traits/same_traits.hpp>
#endif

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // ublas::matrix<>
  template <typename T, typename F, typename ArrT>
  struct matrix_traits<boost::numeric::ublas::matrix<T, F, ArrT> > 
  {
  private:
#ifdef BOOST_BINDINGS_FORTRAN
    BOOST_STATIC_ASSERT((boost::is_same<
      typename F::orientation_category, 
      boost::numeric::ublas::column_major_tag
    >::value)); 
#endif 
  public: 
    typedef T value_type; 
    typedef T* pointer; 
    typedef boost::numeric::ublas::matrix<T, F, ArrT> matrix_type;
    static pointer storage (matrix_type& m) {
      typedef typename matrix_type::array_type array_type; 
      return vector_traits<array_type>::storage (m.data()); 
    }
    static int size1 (matrix_type& m) { return m.size1(); } 
    static int size2 (matrix_type& m) { return m.size2(); }

    static int storage_size(matrix_type& m) 
    { return size1( m ) * size2( m ) ; }

    // stride1 == distance (m (i, j), m (i+1, j)) 
    static int stride1 (matrix_type& m) { 
      return matrix_type::functor_type::one1 (m.size1(), m.size2());
    } 
    // stride2 == distance (m (i, j), m (i, j+1)) 
    static int stride2 (matrix_type& m) { 
      return matrix_type::functor_type::one2 (m.size1(), m.size2());
    }
    static int leading_dimension (matrix_type& m) {
      return matrix_type::functor_type::size2 (m.size1(), m.size2());
    }
    typedef typename detail::ublas_ordering<
      typename F::orientation_category
    >::type ordering_type; 
  }; 

  template <typename T, typename F, typename ArrT>
  struct matrix_traits<boost::numeric::ublas::matrix<T, F, ArrT> const> 
  {
  private:
#ifdef BOOST_BINDINGS_FORTRAN
    BOOST_STATIC_ASSERT((boost::is_same<
      typename F::orientation_category, 
      boost::numeric::ublas::column_major_tag
    >::value)); 
#endif 
  public:
    typedef T value_type; 
    typedef T const* pointer; 
    typedef boost::numeric::ublas::matrix<T, F, ArrT> matrix_type;
    static pointer storage (matrix_type const& m) {
      typedef typename matrix_type::const_array_type const array_type; 
      return vector_traits<array_type>::storage (m.data()); 
    }
    static int size1 (matrix_type const& m) { return m.size1(); } 
    static int size2 (matrix_type const& m) { return m.size2(); }

    static int storage_size(matrix_type const& m) 
    { return size1( m ) * size2( m ) ; }

    static int stride1 (matrix_type const& m) { 
      return matrix_type::functor_type::one1 (m.size1(), m.size2());
    } 
    static int stride2 (matrix_type const& m) { 
      return matrix_type::functor_type::one2 (m.size1(), m.size2());
    }
    static int leading_dimension (matrix_type const& m) {
      return matrix_type::functor_type::size2 (m.size1(), m.size2());
    }
    typedef typename detail::ublas_ordering<
      typename F::orientation_category
    >::type ordering_type; 
  }; 

  // ublas::matrix_symmetric<>
  template <typename T, typename U, typename F, typename ArrT>
  struct matrix_traits<boost::numeric::ublas::symmetric_matrix<T, U, F, ArrT> > 
  {
  private:
#ifdef BOOST_BINDINGS_FORTRAN
    BOOST_STATIC_ASSERT((boost::is_same<
      typename F::orientation_category, 
      boost::numeric::ublas::column_major_tag
    >::value)); 
#endif 
  public: 
    typedef T value_type; 
    typedef T* pointer; 
    typedef boost::numeric::ublas::symmetric_matrix<T, U, F, ArrT> matrix_type;
    static pointer storage (matrix_type& m) {
      typedef typename matrix_type::array_type array_type; 
      return vector_traits<array_type>::storage (m.data()); 
    }
    static int size1 (matrix_type& m) { return m.size1(); } 
    static int size2 (matrix_type& m) { return m.size2(); }

    static int storage_size(matrix_type& m) 
    { return ( size1( m ) + 1 ) * size2( m ) / 2 ; }

    // stride1 == distance (m (i, j), m (i+1, j)) 
    static int stride1 (matrix_type& m) { 
      return (typename matrix_type::functor_type)::one1 (m.size1(), m.size2());
    } 
    // stride2 == distance (m (i, j), m (i, j+1)) 
    static int stride2 (matrix_type& m) { 
      return (typename matrix_type::functor_type)::one2 (m.size1(), m.size2());
    }
    static int leading_dimension (matrix_type& m) {
      return (typename matrix_type::functor_type)::size2 (m.size1(), m.size2());
    }
    typedef typename detail::ublas_ordering<
      typename F::orientation_category
    >::type ordering_type; 
  }; 

  template <typename T, typename U, typename F, typename ArrT>
  struct matrix_traits<boost::numeric::ublas::symmetric_matrix<T, U, F, ArrT> const> 
  {
  private:
#ifdef BOOST_BINDINGS_FORTRAN
    BOOST_STATIC_ASSERT((boost::is_same<
      typename F::orientation_category, 
      boost::numeric::ublas::column_major_tag
    >::value)); 
#endif 
  public:
    typedef T value_type; 
    typedef T const* pointer; 
    typedef boost::numeric::ublas::symmetric_matrix<T, U, F, ArrT> matrix_type;
    static pointer storage (matrix_type const& m) {
      typedef typename matrix_type::const_array_type const array_type; 
      // toon: need const array_type to take correct trait in next line
      return vector_traits<array_type>::storage (m.data()); 
    }
    static int size1 (matrix_type const& m) { return m.size1(); } 
    static int size2 (matrix_type const& m) { return m.size2(); }
    static int storage_size(matrix_type const& m) 
    { return ( size1( m ) + 1 ) * size2( m ) / 2 ; }

    static int stride1 (matrix_type const& m) { 
      return (typename matrix_type::functor_type)::one1 (m.size1(), m.size2());
    } 
    static int stride2 (matrix_type const& m) { 
      return (typename matrix_type::functor_type)::one2 (m.size1(), m.size2());
    }
    static int leading_dimension (matrix_type const& m) {
      return (typename matrix_type::functor_type)::size2 (m.size1(), m.size2());
    }
    typedef typename detail::ublas_ordering<
      typename F::orientation_category
    >::type ordering_type; 
  }; 


  // ublas::matrix_range<> 
  template <typename M>
  struct matrix_traits<boost::numeric::ublas::matrix_range<M> > 
  {
    typedef boost::numeric::ublas::matrix_range<M> matrix_type;
    typedef typename M::value_type value_type;
    typedef value_type* pointer; 

  private:
    typedef typename matrix_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t> cl_traits; 

  public:
    static pointer storage (matrix_type& mr) {
      typename cl_traits::type& mt = cl_traits::get (mr.data()); 
      pointer ptr = matrix_traits<M>::storage (mt);
      ptr += mr.start1() * matrix_traits<M>::stride1 (mt); 
      ptr += mr.start2() * matrix_traits<M>::stride2 (mt); 
      return ptr; 
    }
    static int size1 (matrix_type& mr) { return mr.size1(); } 
    static int size2 (matrix_type& mr) { return mr.size2(); }
    static int stride1 (matrix_type& mr) { 
      return matrix_traits<M>::stride1 (cl_traits::get (mr.data())); 
    } 
    static int stride2 (matrix_type& mr) { 
      return matrix_traits<M>::stride2 (cl_traits::get (mr.data())); 
    }
    static int leading_dimension (matrix_type& mr) {
      return matrix_traits<M>::leading_dimension (cl_traits::get (mr.data())); 
    }
    typedef typename detail::ublas_ordering<
      typename M::orientation_category
    >::type ordering_type; 
  }; 

  template <typename M>
  struct matrix_traits<boost::numeric::ublas::matrix_range<M> const> 
  {
    typedef boost::numeric::ublas::matrix_range<M> matrix_type;
    typedef typename M::value_type value_type;
    typedef value_type const* pointer; 

  private:
    typedef matrix_type const c_matrix_type; 
    typedef typename c_matrix_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t const> cl_traits; 

  public:
    static pointer storage (matrix_type const& mr) {
      typename cl_traits::type const& mt = cl_traits::get (mr.data()); 
      pointer ptr = matrix_traits<M const>::storage (mt);
      ptr += mr.start1() * matrix_traits<M const>::stride1 (mt); 
      ptr += mr.start2() * matrix_traits<M const>::stride2 (mt); 
      return ptr; 
    }
    static int size1 (matrix_type const& mr) { return mr.size1(); } 
    static int size2 (matrix_type const& mr) { return mr.size2(); }
    static int stride1 (matrix_type const& mr) { 
      return matrix_traits<M const>::stride1 (cl_traits::get (mr.data())); 
    } 
    static int stride2 (matrix_type const& mr) { 
      return matrix_traits<M const>::stride2 (cl_traits::get (mr.data())); 
    }
    static int leading_dimension (matrix_type const& mr) {
      return matrix_traits<M const>::leading_dimension 
	(cl_traits::get (mr.data()));
    }
    typedef typename detail::ublas_ordering<
      typename M::orientation_category
    >::type ordering_type; 
  }; 


  // ublas::matrix_slice<> 
  template <typename M>
  struct matrix_traits<boost::numeric::ublas::matrix_slice<M> > 
  {
  private:
    static int ld (int s1, int s2, boost::numeric::ublas::row_major_tag) {
      return s1; 
    }
    static int ld (int s1, int s2, boost::numeric::ublas::column_major_tag) {
      return s2; 
    }
  public:
    typedef boost::numeric::ublas::matrix_slice<M> matrix_type;
    typedef typename M::value_type value_type;
    typedef value_type* pointer; 

  private:
    typedef typename matrix_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t> cl_traits; 

  public:
    static pointer storage (matrix_type& ms) {
      typename cl_traits::type& mt = cl_traits::get (ms.data()); 
      pointer ptr = matrix_traits<M>::storage (mt);
      ptr += ms.start1() * matrix_traits<M>::stride1 (mt); 
      ptr += ms.start2() * matrix_traits<M>::stride2 (mt); 
      return ptr; 
    }
    static int size1 (matrix_type& ms) { return ms.size1(); } 
    static int size2 (matrix_type& ms) { return ms.size2(); }
    static int stride1 (matrix_type& ms) { 
      return ms.stride1() 
	* matrix_traits<M>::stride1 (cl_traits::get (ms.data())); 
    } 
    static int stride2 (matrix_type& ms) { 
      return ms.stride2() 
	* matrix_traits<M>::stride2 (cl_traits::get (ms.data())); 
    }
    static int leading_dimension (matrix_type& ms) {
      typedef typename matrix_type::orientation_category oc_t; 
      return ld (ms.stride1(), ms.stride2(), oc_t())
	* matrix_traits<M>::leading_dimension 
	(cl_traits::get (ms.data())); 
    }
    typedef typename detail::ublas_ordering<
      typename M::orientation_category
    >::type ordering_type; 
  }; 

  template <typename M>
  struct matrix_traits<boost::numeric::ublas::matrix_slice<M> const> 
  {
  private:
    static int ld (int s1, int s2, boost::numeric::ublas::row_major_tag) {
      return s1; 
    }
    static int ld (int s1, int s2, boost::numeric::ublas::column_major_tag) {
      return s2; 
    }
  public:
    typedef boost::numeric::ublas::matrix_slice<M> matrix_type;
    typedef typename M::value_type value_type;
    typedef value_type const* pointer; 

  private:
    typedef matrix_type const c_matrix_type; 
    typedef typename c_matrix_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t const> cl_traits; 

  public:
    static pointer storage (matrix_type const& ms) {
      typename cl_traits::type const& mt = cl_traits::get (ms.data()); 
      pointer ptr = matrix_traits<M const>::storage (mt);
      ptr += ms.start1() * matrix_traits<M const>::stride1 (mt); 
      ptr += ms.start2() * matrix_traits<M const>::stride2 (mt); 
      return ptr; 
    }
    static int size1 (matrix_type const& ms) { return ms.size1(); } 
    static int size2 (matrix_type const& ms) { return ms.size2(); }
    static int stride1 (matrix_type const& ms) { 
      return ms.stride1() 
	* matrix_traits<M const>::stride1 (cl_traits::get (ms.data())); 
    } 
    static int stride2 (matrix_type const& ms) { 
      return ms.stride2() 
	* matrix_traits<M const>::stride2 (cl_traits::get (ms.data())); 
    }
    static int leading_dimension (matrix_type const& ms) {
      typedef typename matrix_type::orientation_category oc_t; 
      return ld (ms.stride1(), ms.stride2(), oc_t())
	* matrix_traits<M>::leading_dimension 
	(cl_traits::get (ms.data())); 
    }
    typedef typename detail::ublas_ordering<
      typename M::orientation_category
    >::type ordering_type; 
  }; 


  // matrix_row<> and matrix_column<> are vectors:
  // ublas::matrix_row<>
  template <typename M>
  struct vector_traits<boost::numeric::ublas::matrix_row<M> > 
  : default_vector_traits<boost::numeric::ublas::matrix_row<M> > 
  {
    typedef boost::numeric::ublas::matrix_row<M> vector_type; 
    typedef typename default_vector_traits<vector_type>::pointer pointer; 

  private:
    typedef typename vector_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t> cl_traits; 

  public:
    static pointer storage (vector_type& mr) {
      typename cl_traits::type& vt = cl_traits::get (mr.data()); 
      pointer ptr = matrix_traits<M>::storage (vt); 
      ptr += mr.index() * matrix_traits<M>::stride1 (vt);
      return ptr; 
    }
    static int stride (vector_type& mr) { 
      return matrix_traits<M>::stride2 (cl_traits::get (mr.data()));
    } 
  }; 

  template <typename M>
  struct vector_traits<boost::numeric::ublas::matrix_row<M> const> 
  : default_vector_traits<boost::numeric::ublas::matrix_row<M> const> 
  {
    typedef boost::numeric::ublas::matrix_row<M> vector_type; 
    typedef 
      typename default_vector_traits<vector_type const>::pointer pointer; 

  private:
    typedef vector_type const c_vector_type; 
    typedef typename c_vector_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t const> cl_traits; 

  public:
    static pointer storage (vector_type const& mr) {
      typename cl_traits::type const& vt = cl_traits::get (mr.data()); 
      pointer ptr = matrix_traits<M const>::storage (vt); 
      ptr += mr.index() * matrix_traits<M const>::stride1 (vt);
      return ptr; 
    }
    static int stride (vector_type const& mr) { 
      return matrix_traits<M const>::stride2 (cl_traits::get (mr.data()));
    } 
  }; 


  // ublas::matrix_column<>
  template <typename M>
  struct vector_traits<boost::numeric::ublas::matrix_column<M> > 
  : default_vector_traits<boost::numeric::ublas::matrix_column<M> > 
  {
    typedef boost::numeric::ublas::matrix_column<M> vector_type; 
    typedef typename default_vector_traits<vector_type>::pointer pointer; 

  private:
    typedef typename vector_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t> cl_traits; 

  public:
    static pointer storage (vector_type& mc) {
      typename cl_traits::type& vt = cl_traits::get (mc.data()); 
      pointer ptr = matrix_traits<M>::storage (vt); 
      ptr += mc.index() * matrix_traits<M>::stride2 (vt);
      return ptr; 
    }
    static int stride (vector_type& mc) { 
      return matrix_traits<M>::stride1 (cl_traits::get (mc.data()));
    } 
  }; 

  template <typename M>
  struct vector_traits<boost::numeric::ublas::matrix_column<M> const> 
  : default_vector_traits<boost::numeric::ublas::matrix_column<M> const> 
  {
    typedef boost::numeric::ublas::matrix_column<M> vector_type; 
    typedef 
      typename default_vector_traits<vector_type const>::pointer pointer; 

  private:
    typedef vector_type const c_vector_type; 
    typedef typename c_vector_type::matrix_closure_type closure_t; 
    typedef detail::closure_traits<closure_t const> cl_traits; 

  public:
    static pointer storage (vector_type const& mc) {
      typename cl_traits::type const& vt = cl_traits::get (mc.data()); 
      pointer ptr = matrix_traits<M const>::storage (vt); 
      ptr += mc.index() * matrix_traits<M const>::stride2 (vt);
      return ptr; 
    }
    static int stride (vector_type const& mc) { 
      return matrix_traits<M const>::stride1 (cl_traits::get (mc.data()));
    } 
  }; 


  // TO DO: matrix_vector_range<>, matrix_vector_slice<> 

}}}}

#endif 
