#if !defined(PROPERTY_DATA_REFERENCE_HPP)
#define PROPERTY_DATA_REFERENCE_HPP

#include "property_reference_interface.hpp"

namespace properties
{

  template <class T, T *d>
  struct prop_data_reference : 
    i_property_reference_read<T>
    {
    
    operator const T & () const 
      { 
      return(get()); 
      }
      
    operator T & () 
      { 
      return(get()); 
      }
    
    const T & get() const 
      { 
      return(*d); 
      }
      
    T & get() 
      { 
      return(*d); 
      }
    
    };
  
}

#endif
