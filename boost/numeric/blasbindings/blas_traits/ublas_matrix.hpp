// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_BLAS_TRAITS_UBLAS_MATRIX_H
#define BOOST_BLASBINDINGS_BLAS_TRAITS_UBLAS_MATRIX_H

#include <boost/numeric/blasbindings/blas_traits.hpp>
#include <boost/numeric/blasbindings/storage_traits/ublas_matrix.hpp> 

namespace boost { namespace numeric { namespace blasbindings {

  // ublas::matrix<>
  // generic version of blas_matrix_traits<> is OK

  // matrix_row<> and column<> are vectors:

  // ublas::matrix_row<>
  template <typename M>
  struct blas_vector_traits<boost::numeric::ublas::matrix_row<M> > {
    typedef boost::numeric::ublas::matrix_row<M> vector_type; 
    typedef typename vector_type::value_type              value_type;
    typedef typename storage_traits<vector_type>::pointer pointer; 
    typedef typename M::functor_type                      functor_type ;
    static pointer storage (vector_type& mr) {
      return storage_traits<vector_type>::storage (mr); 
    }
    static int size (vector_type& mr) { return mr.size(); } 
    static int stride (vector_type& mr) { 
      return functor_type::one2 (mr.data().size1(), mr.data().size2()) ;
    } 
  }; 
  template <typename M>
  struct blas_vector_traits<boost::numeric::ublas::matrix_row<M> const> {
    typedef boost::numeric::ublas::matrix_row<M> vector_type; 
    typedef typename vector_type::value_type                    value_type;
    typedef typename storage_traits<vector_type const>::pointer pointer; 
    typedef typename M::functor_type                            functor_type ;
    static pointer storage (vector_type const& mr) {
      return storage_traits<vector_type const>::storage (mr); 
    }
    static int size (vector_type const& mr) { return mr.size(); } 
    static int stride (vector_type const& mr) { 
      return functor_type::one2 (mr.data().size1(), mr.data().size2()) ;
    } 
  }; 


  // ublas::matrix_column<>
  template <typename M>
  struct blas_vector_traits<boost::numeric::ublas::matrix_column<M> > {
    typedef boost::numeric::ublas::matrix_column<M> vector_type; 
    typedef typename vector_type::value_type              value_type;
    typedef typename storage_traits<vector_type>::pointer pointer; 
    typedef typename M::functor_type                      functor_type ;
    static pointer storage (vector_type& mc) {
      return storage_traits<vector_type>::storage (mc); 
    }
    static int size (vector_type& mc) { return mc.size(); } 
    static int stride (vector_type& mc) { 
      return functor_type::one1 (mc.data().size1(), mc.data().size2()) ;
    } 
  }; 
  template <typename M>
  struct blas_vector_traits<boost::numeric::ublas::matrix_column<M> const> {
    typedef boost::numeric::ublas::matrix_column<M> vector_type; 
    typedef typename vector_type::value_type                    value_type;
    typedef typename storage_traits<vector_type const>::pointer pointer; 
    typedef typename M::functor_type                            functor_type ;
    static pointer storage (vector_type const& mc) {
      return storage_traits<vector_type const>::storage (mc); 
    }
    static int size (vector_type const& mc) { return mc.size(); } 
    static int stride (vector_type const& mc) { 
      return functor_type::one1 (mc.data().size1(), mc.data().size2()) ;
    } 
  }; 

}}}

#endif // BOOST_BLASBINDINGS_BLAS_TRAITS_UBLAS_MATRIX_H
