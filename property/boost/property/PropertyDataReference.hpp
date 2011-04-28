#pragma once

#include "PropertyReferenceInterface.h"

namespace properties
{

  template <class T, T *d>
  struct propDataReference : 
    IPropertyReferenceRead<T>
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
