// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_STORAGE_TRAITS_STD_VECTOR_H
#define BOOST_BLASBINDINGS_STORAGE_TRAITS_STD_VECTOR_H

#include <boost/numeric/blasbindings/storage_traits.hpp>
#include <vector>

namespace boost { namespace numeric { namespace blasbindings {

  // std::vector<>
  template <typename T, typename Alloc>
  struct storage_traits<std::vector<T, Alloc> > {
    typedef typename std::vector<T, Alloc>::pointer pointer; 
    static pointer storage (std::vector<T, Alloc>& v) {
      return &v.front(); 
    }
  }; 
  template <typename T, typename Alloc>
  struct storage_traits<std::vector<T, Alloc> const> {
    typedef typename std::vector<T, Alloc>::const_pointer pointer; 
    static pointer storage (std::vector<T, Alloc> const& v) {
      return &v.front(); 
    }
  }; 

}}}  

#endif // BOOST_BLASBINDINGS_STORAGE_TRAITS_STD_VECTOR_H

