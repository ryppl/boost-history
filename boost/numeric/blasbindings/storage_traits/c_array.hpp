// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_STORAGE_TRAITS_C_ARRAY_H
#define BOOST_BLASBINDINGS_STORAGE_TRAITS_C_ARRAY_H

#include <boost/numeric/blasbindings/storage_traits.hpp>

namespace boost { namespace numeric { namespace blasbindings {

  // built-in array
  template <typename T, std::size_t N> 
  struct storage_traits<T[N]> {
    typedef T* pointer; 
    static pointer storage (T (&a)[N]) { return a; }
  }; 
  template <typename T, std::size_t N> 
  struct storage_traits<T const [N]> {
    typedef T const* pointer;
    static pointer storage (T const (&a)[N]) { return a; }
  }; 

}}}  

#endif // BOOST_BLASBINDINGS_STORAGE_TRAITS_C_ARRAY_H

