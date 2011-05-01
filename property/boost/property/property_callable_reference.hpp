#if !defined(PROPERTY_CALLABLE_REFERENCE_HPP)
#define PROPERTY_CALLABLE_REFERENCE_HPP

#include "PropertyReferenceInterface.h"
#include "PropertyException.h"
#include <boost/function.hpp>

namespace properties
{
  template <class T>
  class prop_callable_reference : 
    public i_property_reference_read<T>
  {

    public:
  
    prop_callable_reference(boost::function<T & ()> g) : 
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
        throw property_read_exception(); 
        } 
      return(fg()); 
      }
      
    T & get() 
      { 
      if (fg == 0) 
        { 
        throw property_read_exception(); 
        } 
      return(fg()); 
      }
  
    private:
  
    boost::function<T & ()> fg;

  };

}

#endif