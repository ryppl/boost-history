// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_STORAGE_TRAITS_UBLAS_H
#define BOOST_BLASBINDINGS_STORAGE_TRAITS_UBLAS_H

#include <cstddef> 
#include <boost/numeric/blasbindings/storage_traits.hpp> 
#include <boost/numeric/ublas/storage.hpp>

namespace boost { namespace numeric { namespace blasbindings {

  // ublas::unbounded<> 
  template <typename T>
  struct storage_traits<boost::numeric::ublas::unbounded_array<T> > {
    typedef boost::numeric::ublas::unbounded_array<T> allocator_type;
    typedef typename allocator_type::pointer          pointer; 
    typedef typename allocator_type::const_pointer    const_pointer; 

    static pointer storage (allocator_type& a) 
    { return a.begin() ; }

  }; 

  template <typename T>
  struct storage_traits<boost::numeric::ublas::unbounded_array<T> const> {
    typedef boost::numeric::ublas::unbounded_array<T> const allocator_type;
    typedef typename allocator_type::const_pointer          pointer; 
    typedef typename allocator_type::const_pointer          const_pointer; 

    static pointer storage (allocator_type& a) 
    { return a.begin() ; }

  }; 

}}}  

#endif // BOOST_BLASBINDINGS_STORAGE_TRAITS_UBLAS_H
