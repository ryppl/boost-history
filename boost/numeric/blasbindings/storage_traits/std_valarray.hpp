// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_STORAGE_TRAITS_STD_VALARRAY_H
#define BOOST_BLASBINDINGS_STORAGE_TRAITS_STD_VALARRAY_H

#include <boost/numeric/blasbindings/storage_traits.hpp>
#include <valarray>

namespace boost { namespace numeric { namespace blasbindings {

  // std::valarray<>
  template <typename T> 
  struct storage_traits<std::valarray<T> > {
    typedef T* pointer; 
    static pointer storage (std::valarray<T>& va) {
      return &va[0]; 
    }
  }; 
  template <typename T> 
  struct storage_traits<std::valarray<T> const> {
    typedef T const* pointer;
    static pointer storage (std::valarray<T> const& va) {
      std::valarray<T>& ncva = const_cast<std::valarray<T>&> (va);
      return &ncva[0];
    }
  }; 

}}}  

#endif // BOOST_BLASBINDINGS_STORAGE_TRAITS_STD_VALARRAY_H

