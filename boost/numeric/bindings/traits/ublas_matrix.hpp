// (-*- C++ -*- header)  

#ifndef BOOST_BINDINGS_TRAITS_UBLAS_MATRIX_H
#define BOOST_BINDINGS_TRAITS_UBLAS_MATRIX_H

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/ublas/matrix.hpp> 

#ifdef BOOST_BINDINGS_FORTRAN
#  include <boost/static_assert.hpp>
#  include <boost/type_traits/same_traits.hpp>
#endif

namespace boost { namespace numeric { namespace bindings {

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
      typedef const typename matrix_type::const_array_type array_type;  //toon: need const array_type to take correct trait in next line
      return vector_traits<array_type>::storage (m.data()); 
    }
    static int size1 (matrix_type const& m) { return m.size1(); } 
    static int size2 (matrix_type const& m) { return m.size2(); }
    static int stride1 (matrix_type const& m) { 
      return matrix_type::functor_type::one1 (m.size1(), m.size2());
    } 
    static int stride2 (matrix_type const& m) { 
      return matrix_type::functor_type::one2 (m.size1(), m.size2());
    }
    static int leading_dimension (matrix_type const& m) {
      return matrix_type::functor_type::size2 (m.size1(), m.size2());
    }
  }; 


  // ublas::matrix_range<> 
  template <typename M>
  struct matrix_traits<boost::numeric::ublas::matrix_range<M> > 
  {
    typedef boost::numeric::ublas::matrix_range<M> matrix_type;
    typedef typename M::value_type value_type;
    typedef value_type* pointer; 
    static pointer storage (matrix_type& mr) {
      pointer ptr = matrix_traits<M>::storage (mr.data());
      ptr += mr.start1() * matrix_traits<M>::stride1 (mr.data()); 
      ptr += mr.start2() * matrix_traits<M>::stride2 (mr.data()); 
      return ptr; 
    }
    static int size1 (matrix_type& mr) { return mr.size1(); } 
    static int size2 (matrix_type& mr) { return mr.size2(); }
    static int stride1 (matrix_type& mr) { 
      return matrix_traits<M>::stride1 (mr.data()); 
    } 
    static int stride2 (matrix_type& mr) { 
      return matrix_traits<M>::stride2 (mr.data()); 
    }
    static int leading_dimension (matrix_type& mr) {
      return matrix_traits<M>::leading_dimension (mr.data()); 
    }
  }; 

  template <typename M>
  struct matrix_traits<boost::numeric::ublas::matrix_range<M> const> 
  {
    typedef boost::numeric::ublas::matrix_range<M> matrix_type;
    typedef typename M::value_type value_type;
    typedef value_type const* pointer; 
    static pointer storage (matrix_type const& mr) {
      pointer ptr = matrix_traits<M const>::storage (mr.data());
      ptr += mr.start1() * matrix_traits<M const>::stride1 (mr.data()); 
      ptr += mr.start2() * matrix_traits<M const>::stride2 (mr.data()); 
      return ptr; 
    }
    static int size1 (matrix_type const& mr) { return mr.size1(); } 
    static int size2 (matrix_type const& mr) { return mr.size2(); }
    static int stride1 (matrix_type const& mr) { 
      return matrix_traits<M const>::stride1 (mr.data()); 
    } 
    static int stride2 (matrix_type const& mr) { 
      return matrix_traits<M const>::stride2 (mr.data()); 
    }
    static int leading_dimension (matrix_type const& mr) {
      return matrix_traits<M const>::leading_dimension (mr.data()); 
    }
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
    static pointer storage (matrix_type& ms) {
      pointer ptr = matrix_traits<M>::storage (ms.data());
      ptr += ms.start1() * matrix_traits<M>::stride1 (ms.data()); 
      ptr += ms.start2() * matrix_traits<M>::stride2 (ms.data()); 
      return ptr; 
    }
    static int size1 (matrix_type& ms) { return ms.size1(); } 
    static int size2 (matrix_type& ms) { return ms.size2(); }
    static int stride1 (matrix_type& ms) { 
      return ms.stride1() * matrix_traits<M>::stride1 (ms.data()); 
    } 
    static int stride2 (matrix_type& ms) { 
      return ms.stride2() * matrix_traits<M>::stride2 (ms.data()); 
    }
    static int leading_dimension (matrix_type& ms) {
      typedef typename matrix_type::orientation_category oc_t; 
      return ld (ms.stride1(), ms.stride2(), oc_t())
	* matrix_traits<M>::leading_dimension (ms.data()); 
    }
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
    static pointer storage (matrix_type const& ms) {
      pointer ptr = matrix_traits<M const>::storage (ms.data());
      ptr += ms.start1() * matrix_traits<M const>::stride1 (ms.data()); 
      ptr += ms.start2() * matrix_traits<M const>::stride2 (ms.data()); 
      return ptr; 
    }
    static int size1 (matrix_type const& ms) { return ms.size1(); } 
    static int size2 (matrix_type const& ms) { return ms.size2(); }
    static int stride1 (matrix_type const& ms) { 
      return ms.stride1() * matrix_traits<M const>::stride1 (ms.data()); 
    } 
    static int stride2 (matrix_type const& ms) { 
      return ms.stride2() * matrix_traits<M const>::stride2 (ms.data()); 
    }
    static int leading_dimension (matrix_type const& ms) {
      typedef typename matrix_type::orientation_category oc_t; 
      return ld (ms.stride1(), ms.stride2(), oc_t())
	* matrix_traits<M>::leading_dimension (ms.data()); 
    }
  }; 


  // matrix_row<> and column<> are vectors:
  // ublas::matrix_row<>
  template <typename M>
  struct vector_traits<boost::numeric::ublas::matrix_row<M> > 
  : default_vector_traits<boost::numeric::ublas::matrix_row<M> > 
  {
    typedef boost::numeric::ublas::matrix_row<M> vector_type; 
    typedef typename default_vector_traits<vector_type>::pointer pointer; 
    static pointer storage (vector_type& mr) {
      pointer ptr = matrix_traits<M>::storage (mr.data()); 
      ptr += mr.index() * matrix_traits<M>::stride1 (mr.data());
      return ptr; 
    }
    static int stride (vector_type& mr) { 
      return matrix_traits<M>::stride2 (mr.data());
    } 
  }; 

  template <typename M>
  struct vector_traits<boost::numeric::ublas::matrix_row<M> const> 
  : default_vector_traits<boost::numeric::ublas::matrix_row<M> const> 
  {
    typedef boost::numeric::ublas::matrix_row<M> vector_type; 
    typedef 
      typename default_vector_traits<vector_type const>::pointer pointer; 
    static pointer storage (vector_type const& mr) {
      pointer ptr = matrix_traits<M const>::storage (mr.data()); 
      ptr += mr.index() * matrix_traits<M const>::stride1 (mr.data());
      return ptr; 
    }
    static int stride (vector_type const& mr) { 
      return matrix_traits<M const>::stride2 (mr.data());
    } 
  }; 


  // ublas::matrix_column<>
  template <typename M>
  struct vector_traits<boost::numeric::ublas::matrix_column<M> > 
  : default_vector_traits<boost::numeric::ublas::matrix_column<M> > 
  {
    typedef boost::numeric::ublas::matrix_column<M> vector_type; 
    typedef typename default_vector_traits<vector_type>::pointer pointer; 
    static pointer storage (vector_type& mc) {
      pointer ptr = matrix_traits<M>::storage (mc.data()); 
      ptr += mc.index() * matrix_traits<M>::stride2 (mc.data());
      return ptr; 
    }
    static int stride (vector_type& mc) { 
      return matrix_traits<M>::stride1 (mc.data());
    } 
  }; 
  template <typename M>
  struct vector_traits<boost::numeric::ublas::matrix_column<M> const> 
  : default_vector_traits<boost::numeric::ublas::matrix_column<M> const> 
  {
    typedef boost::numeric::ublas::matrix_column<M> vector_type; 
    typedef 
      typename default_vector_traits<vector_type const>::pointer pointer; 
    static pointer storage (vector_type const& mc) {
      pointer ptr = matrix_traits<M const>::storage (mc.data()); 
      ptr += mc.index() * matrix_traits<M const>::stride2 (mc.data());
      return ptr; 
    }
    static int stride (vector_type const& mc) { 
      return matrix_traits<M const>::stride1 (mc.data());
    } 
  }; 

}}}

#endif // BOOST_BINDINGS_TRAITS_UBLAS_MATRIX_H
