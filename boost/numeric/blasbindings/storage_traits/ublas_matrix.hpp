// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_STORAGE_TRAITS_UBLAS_MATRIX_H
#define BOOST_BLASBINDINGS_STORAGE_TRAITS_UBLAS_MATRIX_H

#include <cstddef> 
#include <boost/numeric/blasbindings/storage_traits.hpp> 
#include <boost/numeric/ublas/matrix.hpp>

#ifdef BOOST_BLASBINDINGS_CHECK_COLUMN_MAJOR
#  include <boost/static_assert.hpp>
#  include <boost/type_traits/same_traits.hpp>
#endif

namespace boost { namespace numeric { namespace blasbindings {

  // ublas::matrix<> 
  template <typename T, typename F, typename ArrT>
  struct storage_traits<boost::numeric::ublas::matrix<T, F, ArrT> > {
    typedef boost::numeric::ublas::matrix<T, F, ArrT> matrix_type; 
  private:
#ifdef BOOST_BLASBINDINGS_CHECK_COLUMN_MAJOR
    BOOST_STATIC_ASSERT((boost::is_same<
      typename matrix_type::orientation_category,
      boost::numeric::ublas::column_major_tag
    >::value)); 
#endif 
  public: 
    typedef T* pointer; 
    static pointer storage (matrix_type& m) {
      return storage_traits<ArrT>::storage (m.data()); 
    }
  }; 

  template <typename T, typename F, typename ArrT>
  struct storage_traits<boost::numeric::ublas::matrix<T, F, ArrT> const> {
  private:
    typedef boost::numeric::ublas::matrix<T, F, ArrT> matrix_type; 
  private:
#ifdef BOOST_BLASBINDINGS_CHECK_COLUMN_MAJOR
    BOOST_STATIC_ASSERT((boost::is_same<
      typename matrix_type::orientation_category,
      boost::numeric::ublas::column_major_tag
    >::value)); 
#endif 
  public:
    typedef T const* pointer; 
    static pointer storage (matrix_type const& m) {
      return storage_traits<ArrT const>::storage (m.data()); 
    }
  }; 


  // ublas::matrix_row<>
  template <typename Matr>
  struct storage_traits<boost::numeric::ublas::matrix_row<Matr> > {
    typedef boost::numeric::ublas::matrix_row<Matr> matrix_type; 
    typedef typename storage_traits<Matr>::pointer pointer;
    static pointer storage (matrix_type& m) {
      typedef typename Matr::functor_type functor_type ;
      pointer ptr = storage_traits<Matr>::storage (m.data()); 
      ptr += m.index() 
	* functor_type::one1 (m.data().size1(), m.data().size2()) ;
      return ptr; 
    }
  }; 
  template <typename Matr>
  struct storage_traits<boost::numeric::ublas::matrix_row<Matr> const> {
    typedef boost::numeric::ublas::matrix_row<Matr> matrix_type; 
    typedef typename storage_traits<Matr const>::pointer pointer;
    static pointer storage (matrix_type const& m) {
      typedef typename Matr::functor_type functor_type ;
      pointer ptr = storage_traits<Matr const>::storage (m.data()); 
      ptr += m.index() 
	* functor_type::one1 (m.data().size1(), m.data().size2()) ;
      return ptr; 
    }
  }; 


  // ublas::matrix_column<>
  template <typename Matr>
  struct storage_traits<boost::numeric::ublas::matrix_column<Matr> > {
    typedef boost::numeric::ublas::matrix_column<Matr> matrix_type; 
    typedef typename storage_traits<Matr>::pointer pointer;
    static pointer storage (matrix_type& m) {
      typedef typename Matr::functor_type functor_type ;
      pointer ptr = storage_traits<Matr>::storage (m.data()); 
      ptr += m.index() 
	* functor_type::one2 (m.data().size1(), m.data().size2()) ;
      return ptr; 
    }
  }; 
  template <typename Matr>
  struct storage_traits<boost::numeric::ublas::matrix_column<Matr> const> {
    typedef boost::numeric::ublas::matrix_column<Matr> matrix_type; 
    typedef typename storage_traits<Matr const>::pointer pointer;
    static pointer storage (matrix_type const& m) {
      typedef typename Matr::functor_type functor_type ;
      pointer ptr = storage_traits<Matr const>::storage (m.data()); 
      ptr += m.index() 
	* functor_type::one2 (m.data().size1(), m.data().size2()) ;
      return ptr; 
    }
  }; 

}}}  

#endif // BOOST_BLASBINDINGS_STORAGE_TRAITS_UBLAS_MATRIX_H
