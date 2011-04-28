#if !defined(PROPERTY_PREDICATE_FAILURE_RUNTIME_EXCEPTION_HPP)
#define PROPERTY_PREDICATE_FAILURE_RUNTIME_EXCEPTION_HPP

#include <boost/optional.hpp>
#include <boost/function.hpp>
#include "PropertyInterface.h"
#include <stdexcept>
#include <sstream>

namespace properties
{

  template <class T> class PredicateFailureRuntimeException
    {
    
    protected:
      
    void PredicateFailure(const IPropertyWrite<T> & prop,const boost::function<bool (T)> & f,boost::optional<T> oldValue,T newValue)
      {
      
      std::ostringstream oss;
      
      oss << "Error trying to update a writeable property with the value '" << newValue << "'.";
            
      throw std::runtime_error(oss.str());
      }
    
    };

}

#endif
