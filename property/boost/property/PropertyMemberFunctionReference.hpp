#pragma once

#include "PropertyReferenceInterface.h"
#include "PropertyException.h"

namespace properties
{

  template <class T, class C, T & (C::*g)()>
  class propMemberFunctionReference : 
    public IPropertyReferenceRead<T>
  {
    public:
    
    propMemberFunctionReference(C & c) : 
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
      if (g == 0) 
        { 
        throw PropertyReadException(); 
        } 
      return((cf.*g)()); 
      }
      
    T & get() 
      { 
      if (g == 0) 
        { 
        throw PropertyReadException(); 
        } 
      return((cf.*g)()); 
      }
    
    private:
    
    C & cf;
    
  };

}
