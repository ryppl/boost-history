#pragma once

#include "PropertyReferenceInterface.h"
#include "PropertyException.h"

namespace properties
{

  template <class T, T & (*g)()>
  struct propFunctionReference : 
    IPropertyReferenceRead<T>
  {
  
    operator T const & () const 
      { 
      return(get()); 
      }
      
    operator T & () 
      { 
      return(get()); 
      }
    
    T const & get() const 
      { 
      if (g == 0) 
        { 
        throw PropertyReadException(); 
        } 
      return((*g)()); 
      }
      
    T & get() 
      { 
      if (g == 0) 
        { 
        throw PropertyReadException(); 
        } 
      return((*g)()); 
      }
    
  };

}
