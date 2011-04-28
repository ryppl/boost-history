#pragma once

#include "PropertyReferenceInterface.h"

namespace properties
{

  template <class T, class C>
  class propFunctorReference : 
    public IPropertyReferenceRead<T>
  {
  
    public:
    
    propFunctorReference(C & c) : 
      cf(c) 
      { 
      }
    
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
      return(cf()); 
      }
      
    T & get() const 
      { 
      return(cf()); 
      }
    
    private:
    
    C & cf;
    
  };

}
