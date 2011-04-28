#pragma once

#include "PropertyReferenceInterface.h"
#include <boost/utility/value_init.hpp>

namespace properties
{

  template <class T>
  class propAutoReference : public IPropertyReferenceRead<T>
  {
  
    public:

    propAutoReference() { }
    explicit propAutoReference(T arg) : data(arg) { }
    
    operator const T & () const { return(get()); }
    operator T & () { return(get()); }

    const T & get() const { return(boost::get(data)); }
    T & get() { return(boost::get(data)); }
    
    protected:

    boost::initialized<T> data;
    
  };
  
}
