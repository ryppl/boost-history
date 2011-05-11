#if !defined(PROPERTY_INDEX_MAP_HPP)
#define PROPERTY_INDEX_MAP_HPP

#include "detail/property_index_map_detail.hpp"

namespace properties
{

  template <class PMAP>
  struct prop_index_map : 
    detail::PropertyIndexMapMakeBase<PMAP>::type
  {
  
    prop_index_map(PMAP & pmap) : 
      detail::PropertyIndexMapMakeBase<PMAP>::type(pmap) 
      { 
      }
    
    private:
    
    prop_index_map(const prop_index_map<PMAP> &);
    prop_index_map & operator = (const prop_index_map<PMAP> &);
    
  };
  
}

#endif
