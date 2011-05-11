#if !defined(PROPERTY_FUNCTOR_REFERENCE_HPP)
#define PROPERTY_FUNCTOR_REFERENCE_HPP

#include "property_reference_interface.hpp"

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
      
    T & get()
      { 
      return(cf());
      }
    
    private:
    
    C & cf;
    
  };

}

#endif
