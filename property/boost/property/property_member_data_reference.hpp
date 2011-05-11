#if !defined(PROPERTY_MEMBER_DATA_REFERENCE_HPP)
#define PROPERTY_MEMBER_DATA_REFERENCE_HPP

#include "property_reference_interface.hpp"

namespace properties
{

  template <class T, class C, T C::*d>
  class prop_member_data_reference : 
    public i_property_reference_read<T>
  {
    public:
    
    prop_member_data_reference(C & c) : 
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

#endif
