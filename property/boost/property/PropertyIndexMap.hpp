#if !defined(PROPERTY_INDEX_MAP_HPP)
#define PROPERTY_INDEX_MAP_HPP

#include "detail/PropertyIndexMapDetail.h"

namespace properties
{

  template <class PMAP>
  struct PropertyIndexMap : 
    detail::PropertyIndexMapMakeBase<PMAP>::type
  {
  
    PropertyIndexMap(PMAP & pmap) : 
      detail::PropertyIndexMapMakeBase<PMAP>::type(pmap) 
      { 
      }
    
    private:
    
    PropertyIndexMap(const PropertyIndexMap<PMAP> &);
    PropertyIndexMap & operator = (const PropertyIndexMap<PMAP> &);
    
  };
  
}

#endif
