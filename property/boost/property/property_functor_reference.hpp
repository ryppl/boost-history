#if !defined(PROPERTY_FUNCTOR_REFERENCE_HPP)
#define PROPERTY_FUNCTOR_REFERENCE_HPP

#include "PropertyReferenceInterface.h"

namespace properties
{

  template <class T, class C>
  class prop_functor_reference : 
    public i_property_reference_read<T>
  {
  
    public:
    
    prop_functor_reference(C & c) : 
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

#endif
