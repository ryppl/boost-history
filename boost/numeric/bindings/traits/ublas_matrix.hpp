// (-*- C++ -*- header)  

/*
 * 
 * Copyright (c) 2002, 2003 Kresimir Fresl, Toon Knapen and Karl Meerbergen
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

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_MATRIX_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_MATRIX_H

#include <boost/numeric/bindings/traits/traits.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#include <boost/numeric/ublas/matrix.hpp> 
#include <boost/numeric/bindings/traits/detail/generate_const.hpp>
#include <boost/numeric/bindings/traits/detail/array_type_traits.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_matrix_closure.hpp>
#include <boost/numeric/bindings/traits/detail/ublas_ordering.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>

#if defined (BOOST_NUMERIC_BINDINGS_FORTRAN) || !defined (BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK)
#  include <boost/static_assert.hpp>
#  include <boost/type_traits/same_traits.hpp>
#  include <boost/mpl/if.hpp> 
#endif

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // ublas::matrix<>
  // Specialization using matrix_detail_traits so that we can specialize for
  // matrix_detail_traits< matrix<T, F, ArrT>, matrix<T, F, ArrT> >
  // matrix_detail_traits< matrix<T, F, ArrT>, matrix<T, F, ArrT> const >
  // at once.
  template <typename T, typename F, typename ArrT, typename M>
  struct matrix_detail_traits< boost::numeric::ublas::matrix<T, F, ArrT>, M > 
  {
    BOOST_STATIC_ASSERT( (::boost::is_same<boost::numeric::ublas::matrix<T, F, ArrT>, typename boost::remove_const<M>::type>::value) );

  private:
#ifdef BOOST_NUMERIC_BINDINGS_FORTRAN
    BOOST_STATIC_ASSERT((boost::is_same<
      typename F::orientation_category, 
      boost::numeric::ublas::column_major_tag
    >::value)); 
#endif 
  public: 
    typedef general_t matrix_structure; 
    typedef T value_type; 
#ifndef BOOST_NO_CV_TEMPLATE_TEMPLATES
    typedef typename detail::generate_const<M,T>::type* pointer; 
    typedef boost::numeric::ublas::matrix<T, F, ArrT> identifier_type ;
    typedef M                                         matrix_type;
    static pointer storage (matrix_type& m) {
      typedef typename detail::array_type_traits<M>::type array_type; 
      return vector_traits<array_type>::storage (m.data()); 
    }
#endif
    static int size1 (matrix_type& m) { return m.size1(); } 
    static int size2 (matrix_type& m) { return m.size2(); }
    static int storage_size (matrix_type& m) { 
      return size1 (m) * size2 (m); 
    }
    // stride1 == distance (m (i, j), m (i+1, j)) 
    static int stride1 (matrix_type& m) { 
      return identifier_type::functor_type::one1 (m.size1(), m.size2());
    } 
    // stride2 == distance (m (i, j), m (i, j+1)) 
    static int stride2 (matrix_type& m) { 
      return identifier_type::functor_type::one2 (m.size1(), m.size2());
    }
    static int leading_dimension (matrix_type& m) {
      return identifier_type::functor_type::size2 (m.size1(), m.size2());
    }
    typedef typename detail::ublas_ordering<
      typename F::orientation_category
    >::type ordering_type; 
  }; 


  // ublas::matrix_range<> 
  template <typename M, typename MR>
  struct matrix_detail_traits<boost::numeric::ublas::matrix_range<M>, MR >
  {
    BOOST_STATIC_ASSERT( (boost::is_same< boost::numeric::ublas::matrix_range<M>, typename boost::remove_const<MR>::type>::value) ) ;
  private: 
    typedef typename matrix_traits<M>::matrix_structure m_struct_t; 

  public:
#ifdef BOOST_NUMERIC_BINDINGS_NO_STRUCTURE_CHECK 
    typedef m_struct_t matrix_structure; 
#else
    typedef typename 
      boost::mpl::if_c<
        boost::is_same<m_struct_t, traits::general_t>::value,
        traits::general_t,
        traits::unknown_structure_t
      >::type matrix_structure; 
#endif 

    typedef boost::numeric::ublas::matrix_range<M>        identifier_type;
    typedef MR                                            matrix_type;
    typedef typename detail::generate_const<MR,M>::type   m_type ;
    typedef typename M::value_type                        value_type;
    typedef typename detail::generate_const<MR,value_type>::type* pointer; 

  private:
    typedef typename MR::matrix_closure_type                                            closure_t; 
    typedef detail::closure_traits< typename detail::generate_const<MR,closure_t>::type >       cl_traits; 

  public:
    static pointer storage (matrix_type& mr) {
      typename cl_traits::type& mt = cl_traits::get (mr.data()); 
      pointer ptr = matrix_traits<m_type>::storage (mt);
      ptr += mr.start1() * matrix_traits<m_type>::stride1 (mt); 
      ptr += mr.start2() * matrix_traits<m_type>::stride2 (mt); 
      return ptr; 
    }
    static int size1 (matrix_type& mr) { return mr.size1(); } 
    static int size2 (matrix_type& mr) { return mr.size2(); }
    static int stride1 (matrix_type& mr) { 
      return matrix_traits<m_type>::stride1 (cl_traits::get (mr.data())); 
    } 
    static int stride2 (matrix_type& mr) { 
      return matrix_traits<m_type>::stride2 (cl_traits::get (mr.data())); 
    }
    static int leading_dimension (matrix_type& mr) {
      typename cl_traits::type& mt = cl_traits::get (mr.data());
      return matrix_traits<m_type>::leading_dimension (mt); 
    }
    typedef typename detail::ublas_ordering<
      typename M::orientation_category
    >::type ordering_type; 
  }; 


  // ublas::matrix_slice<> 
  template <typename M, typename MS>
  struct matrix_detail_traits<boost::numeric::ublas::matrix_slice<M>, MS > 
  {
    BOOST_STATIC_ASSERT( (boost::is_same< boost::numeric::ublas::matrix_slice<M>, typename boost::remove_const<MS>::type>::value) ) ;

  private:
    static int ld (int s1, int s2, boost::numeric::ublas::row_major_tag) {
      return s1; 
    }
    static int ld (int s1, int s2, boost::numeric::ublas::column_major_tag) {
      return s2; 
    }
  public:
    typedef unknown_structure_t                           matrix_structure; 
    typedef MS                                            matrix_type;
    typedef typename detail::generate_const<MS,M>::type   m_type ;
    typedef boost::numeric::ublas::matrix_slice<M>        identifier_type;
    typedef typename M::value_type                        value_type;
    typedef typename detail::generate_const<MS,value_type>::type* pointer; 

  private:
    typedef typename MS::matrix_closure_type                                                  closure_t; 
    typedef detail::closure_traits< typename detail::generate_const<MS,closure_t>::type>      cl_traits ; 

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
	* matrix_traits<m_type>::stride1 (cl_traits::get (ms.data())); 
    } 
    static int stride2 (matrix_type& ms) { 
      return ms.stride2() 
	* matrix_traits<m_type>::stride2 (cl_traits::get (ms.data())); 
    }
    static int leading_dimension (matrix_type& ms) {
      typedef typename identifier_type::orientation_category oc_t; 
      return ld (ms.stride1(), ms.stride2(), oc_t())
	* matrix_traits<m_type>::leading_dimension 
	(cl_traits::get (ms.data())); 
    }
    typedef typename detail::ublas_ordering<
      typename M::orientation_category
    >::type ordering_type; 
  }; 


  // matrix_row<> and matrix_column<> are vectors:
  // ublas::matrix_row<>
  template <typename M, typename MR>
  struct vector_detail_traits<boost::numeric::ublas::matrix_row<M>, MR > 
  : default_vector_traits< MR, typename M::value_type > 
  {
    typedef typename M::value_type                                 value_type ;
    typedef boost::numeric::ublas::matrix_row<M>                   vector_type; 
    typedef typename default_vector_traits<MR,value_type>::pointer pointer; 

  private:
    typedef typename MR::matrix_closure_type closure_t; 
    typedef detail::closure_traits<typename detail::generate_const<MR,closure_t>::type> cl_traits; 
    typedef typename detail::generate_const<MR,M>::type m_type; 

  public:
    static pointer storage (MR& mr) {
      typename cl_traits::type& vt = cl_traits::get (mr.data()); 
      pointer ptr = matrix_traits<m_type>::storage (vt); 
      ptr += mr.index() * matrix_traits<m_type>::stride1 (vt);
      return ptr; 
    }
    static int stride (MR& mr) { 
      return matrix_traits<m_type>::stride2 (cl_traits::get (mr.data()));
    } 
  }; 


  // ublas::matrix_column<>
  template <typename M, typename MC>
  struct vector_detail_traits<boost::numeric::ublas::matrix_column<M>, MC > 
  : default_vector_traits<MC, typename M::value_type > 
  {
    typedef typename M::value_type                                 value_type ;
    typedef boost::numeric::ublas::matrix_column<M>                vector_type; 
    typedef typename default_vector_traits<MC,value_type>::pointer pointer; 

  private:
    typedef typename MC::matrix_closure_type closure_t; 
    typedef detail::closure_traits<typename detail::generate_const<MC,closure_t>::type> cl_traits;
    typedef typename detail::generate_const<MC,M>::type m_type; 

  public:
    static pointer storage (MC& mc) {
      typename cl_traits::type& vt = cl_traits::get (mc.data()); 
      pointer ptr = matrix_traits<m_type>::storage (vt); 
      ptr += mc.index() * matrix_traits<m_type>::stride2 (vt);
      return ptr; 
    }
    static int stride (MC& mc) { 
      return matrix_traits<m_type>::stride1 (cl_traits::get (mc.data()));
    } 
  }; 

  //
  // free functions
  //

  template < typename T, typename F, typename V >
  inline
  T* matrix_storage(boost::numeric::ublas::matrix< T, F, V >& m) {
    return vector_storage( m.data() ) ;
  }

  template < typename T, typename F, typename V >
  inline
  const T* matrix_storage(const boost::numeric::ublas::matrix< T, F, V >& m) {
    return vector_storage( m.data() ) ;
  }

  // VisualAge can't handle return type V::value_type if only container is taken as template argument
  // template < typename V > const typename V::value_type* vector_storage(const boost::numeric::ublas::matrix_row< V >& v) ;
  //
  template < typename T, typename F, typename V >
  inline
  T* vector_storage(boost::numeric::ublas::matrix_row< boost::numeric::ublas::matrix< T, F, V > >& m) {
    return matrix_storage( m.data().expression() ) + m.index() * stride2( m.data() ) ;
  }

  template < typename T, typename F, typename V >
  inline 
  const T* vector_storage(const boost::numeric::ublas::matrix_row< const boost::numeric::ublas::matrix< T, F, V > >& m) {
    return matrix_storage( m.data().expression() ) + m.index() * stride2( m.data() ) ;
  }

  // VisualAge can't handle return type V::value_type if only container is taken as template argument
  // template < typename V > const typename V::value_type* vector_storage(const boost::numeric::ublas::matrix_column< V >& v) ;
  //
  template < typename T, typename F, typename V >
  inline
  T* vector_storage(boost::numeric::ublas::matrix_column< boost::numeric::ublas::matrix< T, F, V > >& m) {
    return matrix_storage( m.data().expression() ) + m.index() * stride1( m.data() ) ;
  }

  template < typename T, typename F, typename V >
  inline 
  const T* vector_storage(const boost::numeric::ublas::matrix_column< const boost::numeric::ublas::matrix< T, F, V > >& m) {
    return matrix_storage( m.data().expression() ) + m.index() * stride1( m.data() ) ;
  }

  template < typename C >
  inline
  int vector_size(boost::numeric::ublas::matrix_row< C >& v) {
    return v.size() ;
  }

  template < typename C >
  inline
  int vector_size(boost::numeric::ublas::matrix_column< C >& v) {
    return v.size() ;
  }

  template < typename C >
  inline
  int vector_stride(boost::numeric::ublas::matrix_row< C >& v) {
    return stride1( v.data() ) ;
  }

  template < typename C >
  inline
  int vector_stride(boost::numeric::ublas::matrix_column< C >& v) {
    return stride2( v.data() ) ;
  }

  // TO DO: matrix_vector_range<>, matrix_vector_slice<> 

}}}}

#else // if defined BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 
#include <boost/numeric/bindings/traits/matrix_raw.hpp>
#endif // BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#endif // BOOST_NUMERIC_BINDINGS_TRAITS_UBLAS_MATRIX_H
