#if !defined(PROPERTY_AUTO_REFERENCE_HPP)
#define PROPERTY_AUTO_REFERENCE_HPP

#include "PropertyReferenceInterface.h"
#include <boost/utility/value_init.hpp>

namespace properties
{

  template <class T>
  class prop_auto_reference : public i_property_reference_read<T>
  {
  
    public:

    prop_auto_reference() { }
    explicit prop_auto_reference(T arg) : data(arg) { }
    
    operator const T & () const { return(get()); }
    operator T & () { return(get()); }

    const T & get() const { return(boost::get(data)); }
    T & get() { return(boost::get(data)); }
    
    protected:

    boost::initialized<T> data;
    
  };
  
}

#endif
