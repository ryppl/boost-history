#if !defined(PROPERTY_FUNCTION_REFERENCE_HPP)
#define PROPERTY_FUNCTION_REFERENCE_HPP

#include "property_reference_interface.hpp"
#include "property_exception.hpp"

namespace properties
{

  template <class T, T & (*g)()>
  struct prop_function_reference : 
    i_property_reference_read<T>
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
        throw property_read_exception(); 
        } 
      return((*g)()); 
      }
      
    T & get() 
      { 
      if (g == 0) 
        { 
        throw property_read_exception(); 
        } 
      return((*g)()); 
      }
    
  };

}

#endif
