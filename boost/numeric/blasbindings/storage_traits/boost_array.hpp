// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_STORAGE_TRAITS_BOOST_ARRAY_H
#define BOOST_BLASBINDINGS_STORAGE_TRAITS_BOOST_ARRAY_H

#include <boost/numeric/blasbindings/storage_traits.hpp>
#include <boost/array.hpp>

namespace boost { namespace numeric { namespace blasbindings {

  // boost::array<>
  template <typename T, std::size_t N> 
  struct storage_traits<boost::array<T, N> > {
    typedef T* pointer; 
    static pointer storage (boost::array<T, N>& ba) {
      return &ba.front(); 
    }
  }; 
  template <typename T, std::size_t N> 
  struct storage_traits<boost::array<T, N> const> {
    typedef T const* pointer;
    static pointer storage (boost::array<T, N> const& ba) {
      return &ba.front(); 
    }
  }; 

}}}  

#endif // BOOST_BLASBINDINGS_STORAGE_TRAITS_BOOST_ARRAY_H

