// (-*- C++ -*- header)  

#ifndef BOOST_BLASBINDINGS_STORAGE_TRAITS_H
#define BOOST_BLASBINDINGS_STORAGE_TRAITS_H

namespace boost { namespace numeric { namespace blasbindings {

  // assumption: iterator==pointer
  // .. e.g. ublas::(un)bounded_array 
  template <typename V>
  struct storage_traits {
    typedef typename V::pointer pointer; 
    static pointer storage (V& v) {
      return v.begin(); 
    }
  }; 
  template <typename V>
  struct storage_traits<V const> {
    typedef typename V::const_pointer pointer; 
    static pointer storage (V const& v) {
      return v.begin(); 
    }
  }; 

  template <typename VorM>
  inline typename storage_traits<VorM>::pointer storage (VorM& vm) {
    return storage_traits<VorM>::storage (vm); 
  }

}}}  

#endif // BOOST_BLASBINDINGS_STORAGE_TRAITS_H
