#pragma once

#include "PropertyReferenceInterface.h"
#include "PropertyException.h"
#include <boost/function.hpp>

namespace properties
{
  template <class T>
  class propCallableReference : 
    public IPropertyReferenceRead<T>
  {

    public:
  
    propCallableReference(boost::function<T & ()> g) : 
      fg(g) 
      { 
      }
  
    operator T & () 
      { 
      return(get()); 
      }
      
    operator T const & () const 
      { 
      return(get()); 
      }
  
    T const & get() const 
      { 
      if (fg == 0) 
        { 
        throw PropertyReadException(); 
        } 
      return(fg()); 
      }
      
    T & get() 
      { 
      if (fg == 0) 
        { 
        throw PropertyReadException(); 
        } 
      return(fg()); 
      }
  
    private:
  
    boost::function<T & ()> fg;

  };

}
