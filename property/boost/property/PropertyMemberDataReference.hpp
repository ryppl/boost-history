#pragma once

#include "PropertyReferenceInterface.h"

namespace properties
{

  template <class T, class C, T C::*d>
  class propMemberDataReference : 
    public IPropertyReferenceRead<T>
  {
    public:
    
    propMemberDataReference(C & c) : 
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
      return(cf.*d); 
      }
      
    T & get() 
      { 
      return(cf.*d); 
      }
                 
    private:
    
    C & cf;
  
  };

}
